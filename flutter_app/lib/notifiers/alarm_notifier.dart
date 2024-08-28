import 'package:flutter/material.dart';
import 'package:tmp_flutter/services/firebase_service.dart';
import 'dart:async';

class AlarmNotifier extends ChangeNotifier {
  final FirebaseService _firebaseService = FirebaseService();
  Map<String, Map<String, dynamic>> _alarms = {};
  bool _isSynced = true; // Default to true
  Map<String, StreamSubscription<bool>> _enabledListeners = {};
  Map<String, Map<String, dynamic>> get alarms => _alarms;
  bool get isSynced => _isSynced;

  void _setupEnabledListener(String alarmId) {
    _enabledListeners[alarmId]?.cancel(); // Cancel any existing listener
    _enabledListeners[alarmId] = _firebaseService
        .listenToAlarmEnabledStatus(alarmId)
        .listen((isEnabled) {
      if (_alarms.containsKey(alarmId)) {
        _alarms[alarmId]!['enabled'] = isEnabled;
        notifyListeners();
      }
    });
  }

  void removeEnabledListener(String alarmId) {
    if (_enabledListeners.containsKey(alarmId)) {
      _enabledListeners[alarmId]?.cancel(); // Cancel the listener
      _enabledListeners.remove(alarmId); // Remove from the map
    }
  }

  Future<void> addOrUpdateAlarm(String alarmId, Map<String, dynamic> alarmData) async {
    _alarms[alarmId] = alarmData;
    await _firebaseService.addOrUpdateAlarm(alarmId, alarmData);
    // Set up the listener for the 'enabled' status of this alarm
    _setupEnabledListener(alarmId);
    // Assuming the Firebase operation is successful, mark as synced
    _isSynced = true;
    notifyListeners();
  }

  Future<void> deleteAlarm(String alarmId) async {
    _alarms.remove(alarmId);
    await _firebaseService.deleteAlarm(alarmId);
    // Remove the listener when the alarm is deleted
    removeEnabledListener(alarmId);
    // Assuming the Firebase operation is successful, mark as synced
    _isSynced = true;
    notifyListeners();
  }

  Future<void> syncAlarms() async {
    final alarms = await _firebaseService.fetchAllAlarms();
    _alarms = alarms;
    _isSynced = true;
    notifyListeners();
  }

  void markAsUnsynced() {
    _isSynced = false;
    notifyListeners();
  }
}
