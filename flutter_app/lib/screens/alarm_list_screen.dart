import 'package:flutter/material.dart';
import 'package:provider/provider.dart';
import 'package:intl/intl.dart';
import 'package:tmp_flutter/notifiers/alarm_notifier.dart';
import 'package:tmp_flutter/notifiers/task_notifier.dart';
import 'package:tmp_flutter/screens/add_edit_screen.dart';

class AlarmListScreen extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: Text('Smart Alarm Clock'),
        actions: [
          Consumer<AlarmNotifier>(
            builder: (context, alarmNotifier, child) {
              return Row(
                children: [
                  Icon(
                    Icons.circle,
                    color: alarmNotifier.isSynced ? Colors.green : Colors.red,
                  ),
                  IconButton(
                    icon: Icon(Icons.sync),
                    onPressed: () async {
                      await Provider.of<AlarmNotifier>(context, listen: false).syncAlarms();
                      await Provider.of<TaskNotifier>(context, listen: false).syncTasks();
                      ScaffoldMessenger.of(context).showSnackBar(
                        SnackBar(content: Text('Alarms synced')),
                      );
                    },
                  ),
                ],
              );
            },
          ),
        ],
      ),
      body: Consumer<AlarmNotifier>(
        builder: (context, alarmNotifier, child) {
          final alarms = alarmNotifier.alarms;

          if (alarms.isEmpty) {
            return Center(
              child: Text('No alarms available'),
            );
          }

          return ListView.builder(
            itemCount: alarms.length,
            itemBuilder: (context, index) {
              final alarmId = alarms.keys.elementAt(index);
              final alarmData = alarms[alarmId]!;

              return Card(
                child: ListTile(
                  title: Text(
                    alarmData['label'],
                    style: TextStyle(fontSize: 18, fontWeight: FontWeight.bold),
                  ),
                  subtitle: Column(
                    crossAxisAlignment: CrossAxisAlignment.start,
                    children: [
                      Text(
                        'Date and Time: ${_formatDateTime(alarmData['time'])}',
                      ),
                      Text('Repeat: ${alarmData['repeat']}'),
                      Text('Sound: ${alarmData['sound']}'),
                      if (_hasSelectedDays(alarmData['days']))
                        Text('Days: ${_formatDays(alarmData['days'])}'),
                    ],
                  ),
                  trailing: Wrap(
                    spacing: 12,
                    children: <Widget>[
                      Switch(
                        value: alarmData['enabled'] ?? true,
                        onChanged: (value) {
                          final updatedAlarmData = Map<String, dynamic>.from(alarmData);
                          updatedAlarmData['enabled'] = value;
                          Provider.of<AlarmNotifier>(context, listen: false)
                              .addOrUpdateAlarm(alarmId, updatedAlarmData);
                          Provider.of<AlarmNotifier>(context, listen: false).markAsUnsynced();
                        },
                      ),
                      IconButton(
                        icon: Icon(Icons.delete),
                        onPressed: () {
                          Provider.of<AlarmNotifier>(context, listen: false).deleteAlarm(alarmId);
                          Provider.of<AlarmNotifier>(context, listen: false).markAsUnsynced();
                        },
                      ),
                    ],
                  ),
                  onTap: () {
                    Navigator.push(
                      context,
                      MaterialPageRoute(
                        builder: (context) => AddEditScreen(alarmId: alarmId, alarmData: alarmData),
                      ),
                    );
                  },
                ),
              );
            },
          );
        },
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
        currentIndex: 0,
        selectedItemColor: Colors.purple,
        onTap: (int index) {
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

  String _formatDateTime(String dateTimeStr) {
    DateTime dateTime = DateTime.parse(dateTimeStr);
    return DateFormat('dd-MM-yyyy | HH:mm').format(dateTime);
  }

  bool _hasSelectedDays(List<dynamic> days) {
    return days.contains(true);
  }

  String _formatDays(List<dynamic> days) {
    const dayLabels = ['Mon', 'Tue', 'Wed', 'Thu', 'Fri', 'Sat', 'Sun'];
    List<String> selectedDays = [];
    for (int i = 0; i < days.length; i++) {
      if (days[i]) {
        selectedDays.add(dayLabels[i]);
      }
    }
    return selectedDays.join(', ');
  }
}
