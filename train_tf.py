import os
import pickle
import numpy as np
import tensorflow as tf
from tensorflow.keras import datasets, layers, models
from tensorflow.keras.callbacks import EarlyStopping, LearningRateScheduler
from sklearn.model_selection import train_test_split

# 데이터셋 파일 경로
data_dir = "/home/nuc/CNN/archive/cifar-10-batches-py"

# CIFAR-10 데이터셋 로드 함수
def load_cifar10_batch(file_path):
    with open(file_path, 'rb') as f:
        batch = pickle.load(f, encoding='bytes')
        data = batch[b'data']
        labels = batch[b'labels']
        # 데이터를 (batch_size, 32, 32, 3) 형태로 변환
        data = data.reshape(-1, 3, 32, 32).transpose(0, 2, 3, 1)
        labels = np.array(labels)
    return data, labels

def load_cifar10(data_dir):
    x_train = []
    y_train = []
    # 훈련 데이터 로드 (data_batch_1 ~ data_batch_5)
    for i in range(1, 6):
        data, labels = load_cifar10_batch(os.path.join(data_dir, f"data_batch_{i}"))
        x_train.append(data)
        y_train.append(labels)
    x_train = np.concatenate(x_train)
    y_train = np.concatenate(y_train)
    # 테스트 데이터 로드 (test_batch)
    x_test, y_test = load_cifar10_batch(os.path.join(data_dir, "test_batch"))
    return (x_train, y_train), (x_test, y_test)

# CIFAR-10 데이터셋 로드
(x_train, y_train), (x_test, y_test) = load_cifar10(data_dir)

# 데이터 정규화
x_train, x_test = x_train / 255.0, x_test / 255.0

# Training 데이터를 Training과 Validation으로 분리 
x_train, x_val, y_train, y_val = train_test_split(
    x_train, y_train, test_size=0.2, random_state=42
)

# TensorFlow Dataset으로 변환
train_ds = tf.data.Dataset.from_tensor_slices((x_train, y_train)).batch(64).prefetch(tf.data.AUTOTUNE)
val_ds = tf.data.Dataset.from_tensor_slices((x_val, y_val)).batch(64).prefetch(tf.data.AUTOTUNE)
test_ds = tf.data.Dataset.from_tensor_slices((x_test, y_test)).batch(64).prefetch(tf.data.AUTOTUNE)

# 모델 생성 (복잡도 감소 및 드롭아웃 증가)

model = tf.keras.models.Sequential([
    tf.keras.layers.Conv2D(32, 3, padding='same', input_shape=x_train.shape[1:], activation='relu'),
    tf.keras.layers.Conv2D(32, 3, activation='relu'),
    tf.keras.layers.MaxPooling2D(),
    tf.keras.layers.Dropout(0.25),

    tf.keras.layers.Conv2D(64, 3, padding='same', activation='relu'),
    tf.keras.layers.Conv2D(64, 3, activation='relu'),
    tf.keras.layers.MaxPooling2D(),
    tf.keras.layers.Dropout(0.25),

    tf.keras.layers.Flatten(),
    tf.keras.layers.Dense(512, activation='relu'),
    tf.keras.layers.Dropout(0.5),
    tf.keras.layers.Dense(10, activation='softmax'),
])



model.summary()

# 학습률 스케줄링 함수
def lr_schedule(epoch, lr):
    if epoch < 10:
        return lr
    else:
        return lr * 0.9

# Early Stopping 설정 (강화)
early_stopping = EarlyStopping(monitor='val_loss', patience=5, restore_best_weights=True)

# 모델 컴파일
model.compile(
    optimizer=tf.keras.optimizers.Adam(learning_rate=0.001),
    loss='sparse_categorical_crossentropy',
    metrics=['accuracy']
)


# model.compile(optimizer=tf.keras.optimizers.RMSprop(learning_rate=0.0001, decay=1e-06),
#             loss='categorical_crossentropy', metrics=['acc'])


# 모델 학습
history = model.fit(
    train_ds,
    epochs=50,
    validation_data=val_ds,
    callbacks=[early_stopping, LearningRateScheduler(lr_schedule)]
)

# 모델 평가
test_loss, test_acc = model.evaluate(test_ds)
print(f"테스트 손실: {test_loss}")
print(f"테스트 정확도: {test_acc}")
