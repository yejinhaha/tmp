import 'package:firebase_core/firebase_core.dart';
import 'package:flutter/foundation.dart';

Future initFirebase() async {
  if (kIsWeb) {
    await Firebase.initializeApp(
        options: const FirebaseOptions(
            apiKey: "AIzaSyBCgAxgSw-4oxn-N1o0q9lI8H3mk1Dezjc",
            authDomain: "login-995b4.firebaseapp.com",
            projectId: "login-995b4",
            storageBucket: "login-995b4.appspot.com",
            messagingSenderId: "1019932424835",
            appId: "1:1019932424835:web:ea7ee9a9897215da0d79b6",
            measurementId: "G-TLYHC1HT44"));
  } else {
    await Firebase.initializeApp();
  }
}
