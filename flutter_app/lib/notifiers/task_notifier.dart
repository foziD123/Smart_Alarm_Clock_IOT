import 'package:flutter/material.dart';
import 'package:tmp_flutter/task_model.dart';
import 'package:tmp_flutter/services/firebase_service.dart';
class TaskNotifier extends ChangeNotifier {
  final FirebaseService _firebaseService = FirebaseService();
  List<Task> _tasks = [];
  bool _isSynced = true; // Default to true

  List<Task> get tasks => _tasks;
  bool get isSynced => _isSynced;

  Future<void> addOrUpdateTask(Task task) async {
    await _firebaseService.saveTask(task.id, task.toMap());
    await fetchTasks();
    _isSynced = true;
    notifyListeners();
  }

  Future<void> deleteTask(String taskId) async {
    await _firebaseService.deleteTask(taskId);
    await fetchTasks();
    _isSynced = true;
    notifyListeners();
  }

  Future<void> deleteAllTasks() async {
    await _firebaseService.deleteAllTasks(); // Call the method in FirebaseService to delete all tasks
    _tasks.clear(); // Clear the local task list
    _isSynced = true; // Mark as synced after deleting
    notifyListeners(); // Notify listeners to update the UI
  }

  Future<void> fetchTasks() async {
    _tasks = await _firebaseService.fetchTasks();
    notifyListeners();
  }

  Future<void> syncTasks() async {
    await fetchTasks();
    _isSynced = true;
    notifyListeners();
  }

  void markAsUnsynced() {
    _isSynced = false;
    notifyListeners();
  }
}
