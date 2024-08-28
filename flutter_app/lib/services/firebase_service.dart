import 'package:firebase_database/firebase_database.dart';
import 'package:tmp_flutter/task_model.dart';

class FirebaseService {
  final DatabaseReference _databaseReference = FirebaseDatabase.instance.ref();

  FirebaseService();

  Future<int?> getSnoozeTime() async {
    final snapshot = await _databaseReference.child('settings/snooze').get();
    if (snapshot.exists) {
      return snapshot.value as int?;
    }
    return null; // Return null if there's no snooze time set
  }

  Future<void> setSnoozeTime(int snoozeTime) async {
    try {
      await _databaseReference.child('settings/snooze').set(snoozeTime);
      print('Snooze time set to: $snoozeTime minutes');
    } catch (e) {
      print('Error setting snooze time: $e');
    }
  }

  Future<void> setVolume(int volume) async {
    try {
      await _databaseReference.child('settings/volume').set(volume);
      print('Volume set to $volume');
    } catch (e) {
      print('Error setting volume: $e');
    }
  }

  Future<int?> getVolume() async {
    final snapshot = await _databaseReference.child('settings/volume').get();
    if (snapshot.exists) {
      return snapshot.value as int?;
    }
    return null; // Return null if no value is found
  }


  Future<void> setBrightness(int brightness) async {
    try {
      await _databaseReference.child('settings/brightness').set(brightness);
      print('Brightness set to $brightness');
    } catch (e) {
      print('Error setting brightness: $e');
    }
  }

  Future<int?> getBrightness() async {
    final snapshot = await _databaseReference.child('settings/brightness').get();
    if (snapshot.exists) {
      return snapshot.value as int?;
    }
    return null; // Return null if no value is found
  }

  Future<void> storeCalendarEvents(List<Map<String, dynamic>> events) async {
    await _databaseReference.child('calendar/events').set(events);
  }

  Future<void> storeTask(Task task) async {
    await _databaseReference.child('tasks').child(task.id).set(task.toMap());
  }

  Future<void> storeTasks(List<Map<String, dynamic>> tasks) async {
    await _databaseReference.child('tasks').set(tasks);
  }

  Future<void> saveTask(String taskId, Map<String, dynamic> taskData) async {
    try {
      await _databaseReference.child('tasks').child(taskId).set(taskData);
      print('Task data added/updated: $taskData');
    } catch (e) {
      print('Error adding/updating task data: $e');
    }
  }

  Future<void> deleteTask(String taskId) async {
    await _databaseReference.child('tasks').child(taskId).remove();
  }

  Future<List<Task>> fetchTasks() async {
    final snapshot = await _databaseReference.child('tasks').get();
    if (snapshot.exists) {
      print('Tasks fetched: ${snapshot.value}');
      final tasksMap = (snapshot.value as Map).cast<String, Map<dynamic, dynamic>>();
      return tasksMap.entries.map((entry) {
        return Task.fromMap(entry.value.cast<String, dynamic>());
      }).toList();
    } else {
      print('No tasks found in Firebase');
    }
    return [];
  }
  Future<void> deleteAllTasks() async {
    try {
      final snapshot = await _databaseReference.child('tasks').get();

      if (snapshot.exists) {
        final tasksMap = snapshot.value as Map<dynamic, dynamic>;

        // Iterate over each task and delete it
        for (var taskId in tasksMap.keys) {
          await _databaseReference.child('tasks').child(taskId).remove();
        }

        print('All tasks deleted from Firebase');
      } else {
        print('No tasks found to delete');
      }
    } catch (e) {
      print('Error deleting all tasks: $e');
    }
  }


  Future<String> getESP32Status() async {
    final snapshot = await _databaseReference.child('esp32/status').get();
    if (snapshot.exists) {
      return snapshot.value.toString();
    }
    return 'unknown';
  }

  Future<DateTime?> getESP32LastTimestamp() async {
    final snapshot = await _databaseReference.child('esp32/status').get();
    if (snapshot.exists) {
      final timestamp = snapshot.value as int?;
      if (timestamp != null) {
        try {
          return DateTime.fromMillisecondsSinceEpoch(timestamp);
        } catch (e) {
          print('Error converting timestamp to DateTime: $e');
        }
      }
    }
    return null;
  }

  Stream<bool> listenToAlarmEnabledStatus(String alarmId) {
    return _databaseReference
        .child('alarms/$alarmId/enabled')
        .onValue
        .map((event) => event.snapshot.value as bool? ?? false);
  }

  Future<void> addOrUpdateAlarm(String alarmId, Map<String, dynamic> alarmData) async {
    try {
      if (!alarmData.containsKey('enabled')) {
        alarmData['enabled'] = true;
      }
      await _databaseReference.child('alarms').child(alarmId).set(alarmData);
      print('Alarm data added/updated: $alarmData');
    } catch (e) {
      print('Error adding/updating alarm data: $e');
    }
  }

  Future<void> deleteAlarm(String alarmId) async {
    try {
      await _databaseReference.child('alarms').child(alarmId).remove();
      print('Alarm data removed: $alarmId');
    } catch (e) {
      print('Error removing alarm data: $e');
    }
  }

  Future<void> clearAllAlarms() async {
    try {
      await _databaseReference.child('alarms').remove();
      print('All alarms cleared');
    } catch (e) {
      print('Error clearing alarms: $e');
    }
  }

  Future<Map<String, Map<String, dynamic>>> fetchAllAlarms() async {
    Map<String, Map<String, dynamic>> alarms = {};
    try {
      final snapshot = await _databaseReference.child('alarms').get();
      if (snapshot.exists && snapshot.value != null) {
        final Map<dynamic, dynamic> alarmMap = snapshot.value as Map<dynamic, dynamic>;
        alarmMap.forEach((key, value) {
          alarms[key] = Map<String, dynamic>.from(value);
        });
      }
    } catch (e) {
      print('Error fetching alarms: $e');
    }
    return alarms;
  }
}
