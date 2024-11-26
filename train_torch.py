import torch
import torch.nn as nn
import torch.optim as optim
from torch.utils.data import DataLoader
import torchvision.transforms as transforms
from datasets import load_dataset
from tqdm import tqdm
import torchsummary
from torch.utils.data import random_split

# CIFAR-100 데이터셋 로드
dataset = load_dataset("uoft-cs/cifar100")

# 데이터 전처리 (최적화된 증강)
transform = transforms.Compose([
    transforms.RandomHorizontalFlip(),
    transforms.RandomCrop(32, padding=4),
    transforms.RandomRotation(5),  # 증강 강도 조정
    transforms.ColorJitter(brightness=0.1, contrast=0.1),  # 증강 강도 조정
    transforms.ToTensor(),
    transforms.Normalize(mean=[0.5, 0.5, 0.5], std=[0.5, 0.5, 0.5])
])





# PyTorch Dataset 정의
class CIFAR100Dataset(torch.utils.data.Dataset):
    def __init__(self, split, transform):
        self.data = dataset[split]
        self.transform = transform

    def __len__(self):
        return len(self.data)

    def __getitem__(self, idx):
        image = self.data[idx]['img']
        label = self.data[idx]['fine_label']
        if self.transform:
            image = self.transform(image)
        return image, label

# Dataset 및 DataLoader 생성
batch_size = 128  # 배치 크기 증가

train_dataset = CIFAR100Dataset(split='train', transform=transform)
test_dataset = CIFAR100Dataset(split='test', transform=transform)

# CIFAR-100 데이터셋에서 train 데이터를 4:1로 나눔
train_size = int(len(train_dataset) * 0.8)  # 80%
val_size = len(train_dataset) - train_size  # 20%
train_dataset, val_dataset = random_split(train_dataset, [train_size, val_size])

# 새로운 DataLoader 생성
train_loader = DataLoader(train_dataset, batch_size=batch_size, shuffle=True)
val_loader = DataLoader(val_dataset, batch_size=batch_size, shuffle=False)
test_loader = DataLoader(test_dataset, batch_size=batch_size, shuffle=False)

# 모델 정의 (Dropout 조정)
class CIFAR100Model(nn.Module):
    def __init__(self):
        super(CIFAR100Model, self).__init__()
        self.model = nn.Sequential(
            nn.Conv2d(3, 64, kernel_size=3, padding=1),  # Conv1
            nn.ReLU(),
            nn.BatchNorm2d(64),
            nn.MaxPool2d(2, 2),  # 32x32 -> 16x16
            nn.Dropout(0.2),  # Dropout 감소

            nn.Conv2d(64, 128, kernel_size=3, padding=1),  # Conv2
            nn.ReLU(),
            nn.BatchNorm2d(128),
            nn.MaxPool2d(2, 2),  # 16x16 -> 8x8
            nn.Dropout(0.3),  # Dropout 감소

            nn.Conv2d(128, 256, kernel_size=3, padding=1),  # Conv3
            nn.ReLU(),
            nn.BatchNorm2d(256),
            nn.MaxPool2d(2, 2),  # 8x8 -> 4x4
            nn.Dropout(0.4),  # 기존 유지

            nn.Flatten(),
            nn.Linear(256 * 4 * 4, 256),  # Fully Connected 1
            nn.ReLU(),
            nn.Dropout(0.5),  # Dropout 감소
            nn.Linear(256, 100)  # Fully Connected 2 (100개 클래스 출력)
        )

    def forward(self, x):
        return self.model(x)

# 모델 초기화
device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
model = CIFAR100Model().to(device)
torchsummary.summary(model,(3,32,32))

# 손실 함수 및 옵티마이저 정의
criterion = nn.CrossEntropyLoss()
optimizer = optim.Adam(model.parameters(), lr=0.0005, weight_decay=1e-3)


# 학습률 스케줄러 수정 (ReduceLROnPlateau)
scheduler = optim.lr_scheduler.ReduceLROnPlateau(optimizer, mode='max', patience=3, factor=0.5)


# EarlyStopping 수정
class EarlyStopping:
    def __init__(self, patience=7, min_accuracy_threshold=0.6, verbose=False):
        self.patience = patience
        self.min_accuracy_threshold = min_accuracy_threshold
        self.verbose = verbose
        self.counter = 0
        self.best_accuracy = None
        self.early_stop = False

    def __call__(self, val_accuracy, model, path='checkpoint.pt'):
        if val_accuracy < self.min_accuracy_threshold:
            return  # EarlyStopping을 작동시키지 않음

        if self.best_accuracy is None:
            self.best_accuracy = val_accuracy
            self.save_checkpoint(val_accuracy, model, path)
        elif val_accuracy <= self.best_accuracy:
            self.counter += 1
            if self.verbose:
                print(f"EarlyStopping counter: {self.counter} out of {self.patience}")
            if self.counter >= self.patience:
                self.early_stop = True
        else:
            self.best_accuracy = val_accuracy
            self.save_checkpoint(val_accuracy, model, path)
            self.counter = 0

    def save_checkpoint(self, val_accuracy, model, path):
        if self.verbose:
            print(f"Validation accuracy improved ({self.best_accuracy:.6f}). Saving model ...")
        torch.save(model.state_dict(), path)

# EarlyStopping 객체 생성
early_stopping = EarlyStopping(patience=7, verbose=True)



# 학습 루프
def train(model, train_loader, criterion, optimizer, device):
    model.train()
    running_loss = 0.0
    correct = 0
    total = 0

    for images, labels in tqdm(train_loader, desc="Training"):
        images, labels = images.to(device), labels.to(device)

        optimizer.zero_grad()
        outputs = model(images)
        loss = criterion(outputs, labels)
        loss.backward()
        optimizer.step()

        running_loss += loss.item() * images.size(0)
        _, predicted = outputs.max(1)
        total += labels.size(0)
        correct += predicted.eq(labels).sum().item()

    epoch_loss = running_loss / total
    accuracy = correct / total
    return epoch_loss, accuracy

def evaluate(model, test_loader, criterion, device):
    model.eval()
    running_loss = 0.0
    correct = 0
    total = 0

    with torch.no_grad():
        for images, labels in tqdm(test_loader, desc="Evaluating"):
            images, labels = images.to(device), labels.to(device)

            outputs = model(images)
            loss = criterion(outputs, labels)

            running_loss += loss.item() * images.size(0)
            _, predicted = outputs.max(1)
            total += labels.size(0)
            correct += predicted.eq(labels).sum().item()

    epoch_loss = running_loss / total
    accuracy = correct / total
    return epoch_loss, accuracy

# 테스트 정확도 측정 함수 추가
def test(model, test_loader, criterion, device):
    model.eval()
    running_loss = 0.0
    correct = 0
    total = 0

    with torch.no_grad():
        for images, labels in tqdm(test_loader, desc="Testing"):
            images, labels = images.to(device), labels.to(device)

            outputs = model(images)
            loss = criterion(outputs, labels)

            running_loss += loss.item() * images.size(0)
            _, predicted = outputs.max(1)
            total += labels.size(0)
            correct += predicted.eq(labels).sum().item()

    test_loss = running_loss / total
    test_accuracy = correct / total
    return test_loss, test_accuracy

# 학습 및 평가
num_epochs = 150  # 에포크 증가
best_val_accuracy = 0.0

# 학습 및 평가 루프 수정
for epoch in range(num_epochs):
    print(f"Epoch {epoch + 1}/{num_epochs}")
    
    train_loss, train_accuracy = train(model, train_loader, criterion, optimizer, device)
    val_loss, val_accuracy = test(model, val_loader, criterion, device) 
    
    print(f"Train Loss: {train_loss:.4f}, Train Accuracy: {train_accuracy:.4f}")
    print(f"Val Loss: {val_loss:.4f}, Val Accuracy: {val_accuracy:.4f}")
    
    scheduler.step(val_accuracy)  # Validation Accuracy 기반으로 학습률 감소
    early_stopping(val_accuracy, model)

    if early_stopping.early_stop:
        print("Early stopping triggered.")
        break

    if val_accuracy > best_val_accuracy:
        best_val_accuracy = val_accuracy

print(f"Best Validation Accuracy: {best_val_accuracy:.4f}")

# 최종 테스트 정확도 측정
print("Evaluating on Test Data...")
test_loss, test_accuracy = test(model, test_loader, criterion, device)
print(f"Test Loss: {test_loss:.4f}, Test Accuracy: {test_accuracy:.4f}")





