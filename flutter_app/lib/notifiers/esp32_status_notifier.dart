import 'package:flutter/material.dart';

class ESP32StatusNotifier extends ChangeNotifier {
  String _status = 'unknown';

  String get status => _status;

  void updateStatus(String newStatus) {
    if (_status != newStatus) {
      _status = newStatus;
      notifyListeners();
    }
  }
}
