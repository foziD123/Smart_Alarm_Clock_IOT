import 'package:flutter/material.dart';
import 'package:provider/provider.dart';
import 'package:tmp_flutter/notifiers/task_notifier.dart';
import 'package:tmp_flutter/notifiers/alarm_notifier.dart';
import 'package:intl/intl.dart';
import 'package:tmp_flutter/screens/task_edit_screen.dart';
import 'package:tmp_flutter/screens/sync_screen.dart';
import 'package:tmp_flutter/screens/home_screen.dart';

class TaskListScreen extends StatefulWidget {
  @override
  _TaskListScreenState createState() => _TaskListScreenState();
}

class _TaskListScreenState extends State<TaskListScreen> {
  bool _sortByDate = true; // Initial sort mode: by date

  void _toggleSortMode() {
    setState(() {
      _sortByDate = !_sortByDate;
    });
  }

  Future<void> _deleteAllTasks(BuildContext context) async {
    final confirm = await showDialog(
      context: context,
      builder: (context) => AlertDialog(
        title: Text('Delete All Tasks'),
        content: Text('Are you sure you want to delete all tasks? This action cannot be undone.'),
        actions: [
          TextButton(
            onPressed: () => Navigator.of(context).pop(false),
            child: Text('Cancel'),
          ),
          TextButton(
            onPressed: () => Navigator.of(context).pop(true),
            child: Text('Delete'),
          ),
        ],
      ),
    );

    if (confirm == true) {
      await Provider.of<TaskNotifier>(context, listen: false).deleteAllTasks();
      ScaffoldMessenger.of(context).showSnackBar(
        SnackBar(content: Text('All tasks have been deleted')),
      );
    }
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: Text('Tasks'),
        actions: [
          Consumer<TaskNotifier>(
            builder: (context, taskNotifier, child) {
              return Row(
                children: [
                  Icon(
                    Icons.circle,
                    color: taskNotifier.isSynced ? Colors.green : Colors.red,
                  ),
                  IconButton(
                    icon: Icon(_sortByDate ? Icons.date_range : Icons.sort),
                    onPressed: _toggleSortMode,
                    tooltip: _sortByDate ? 'Sort by Priority' : 'Sort by Date',
                  ),
                  /*IconButton(
                    icon: Icon(Icons.delete),
                    onPressed: () => _deleteAllTasks(context),
                    tooltip: 'Delete All Tasks',
                  ),*/
                  IconButton(
                    icon: Icon(Icons.sync),
                    onPressed: () async {
                      await Provider.of<TaskNotifier>(context, listen: false).syncTasks();
                      await Provider.of<AlarmNotifier>(context, listen: false).syncAlarms();
                      ScaffoldMessenger.of(context).showSnackBar(
                        SnackBar(content: Text('Alarms and Tasks synced')),
                      );
                    },
                  ),
                ],
              );
            },
          ),
        ],
      ),
      body: Consumer<TaskNotifier>(
        builder: (context, taskNotifier, child) {
          // Sort tasks based on the selected mode
          final tasks = taskNotifier.tasks;
          if (_sortByDate) {
            tasks.sort((a, b) => a.dueDate.compareTo(b.dueDate));
          } else {
            tasks.sort((a, b) => _comparePriority(a.priority, b.priority));
          }

          if (tasks.isEmpty) {
            return Center(
              child: Text('No tasks available'),
            );
          }

          return ListView.builder(
            itemCount: tasks.length,
            itemBuilder: (context, index) {
              final task = tasks[index];
              final color = _getPriorityColor(task.priority);

              return Card(
                color: color,
                child: ListTile(
                  title: Text(
                    task.name,
                    style: TextStyle(
                      fontSize: 18,
                      fontWeight: FontWeight.bold,
                      color: Colors.white,
                    ),
                  ),
                  subtitle: Text(
                    'Due: ${DateFormat('dd-MM-yyyy').format(task.dueDate)} \nPriority: ${task.priority}',
                    style: TextStyle(
                      fontSize: 16,
                      color: Colors.white,
                    ),
                  ),
                  trailing: IconButton(
                    icon: Icon(Icons.delete, color: Colors.white),
                    onPressed: () {
                      Provider.of<TaskNotifier>(context, listen: false).deleteTask(task.id);
                    },
                  ),
                  onTap: () {
                    Navigator.push(
                      context,
                      MaterialPageRoute(
                        builder: (context) => TaskEditScreen(task: task),
                      ),
                    );
                  },
                ),
              );
            },
          );
        },
      ),
      floatingActionButton: FloatingActionButton(
        onPressed: () {
          Provider.of<TaskNotifier>(context, listen: false).markAsUnsynced();
          Navigator.push(
            context,
            MaterialPageRoute(builder: (context) => TaskEditScreen()),
          );
        },
        child: Icon(Icons.add),
      ),
      bottomNavigationBar: BottomNavigationBar(
        items: const <BottomNavigationBarItem>[
          BottomNavigationBarItem(
            icon: Icon(Icons.home),
            label: 'Home',
          ),
          BottomNavigationBarItem(
            icon: Icon(Icons.task),
            label: 'Tasks',
          ),
          BottomNavigationBarItem(
            icon: Icon(Icons.settings),
            label: 'Settings',
          ),
        ],
        currentIndex: 1, // Set the current index to "Tasks"
        selectedItemColor: Colors.purple,
        onTap: (index) {
          switch (index) {
            case 0:
              Navigator.pushNamed(context, '/home');
              break;
            case 1:
              Navigator.pushNamed(context, '/tasks');
              break;
            case 2:
              Navigator.pushNamed(context, '/settings');
              break;
          }
        },
      ),
    );
  }

  // Compare function for sorting by priority
  int _comparePriority(String a, String b) {
    const priorities = {'High': 0, 'Medium': 1, 'Low': 2};
    return priorities[a]!.compareTo(priorities[b]!);
  }

  // Get the color based on priority
  Color _getPriorityColor(String priority) {
    switch (priority) {
      case 'High':
        return Colors.red;
      case 'Medium':
        return Colors.orange;
      case 'Low':
      default:
        return Colors.green;
    }
  }
}
