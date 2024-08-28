import 'package:flutter/material.dart';
import 'package:provider/provider.dart';
import 'package:tmp_flutter/notifiers/alarm_notifier.dart';
import 'package:tmp_flutter/notifiers/task_notifier.dart';

class SyncScreen extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: Text('Sync Data'),
      ),
      body: Center(
        child: ElevatedButton(
          onPressed: () async {
            // Sync Alarms
            await Provider.of<AlarmNotifier>(context, listen: false).syncAlarms();
            // Sync Tasks
            await Provider.of<TaskNotifier>(context, listen: false).fetchTasks();
            // Mark as Synced
            Provider.of<AlarmNotifier>(context, listen: false).markAsUnsynced();
            Provider.of<TaskNotifier>(context, listen: false).markAsUnsynced();
            // Show a message
            ScaffoldMessenger.of(context).showSnackBar(
              SnackBar(content: Text('Data synced successfully')),
            );
            Navigator.pop(context);
          },
          child: Text('Sync Now'),
        ),
      ),
    );
  }
}
