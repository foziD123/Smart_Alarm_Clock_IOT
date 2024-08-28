import 'package:flutter/material.dart';
import 'package:provider/provider.dart';
import 'package:tmp_flutter/notifiers/alarm_notifier.dart';
import 'package:flutter_time_picker_spinner/flutter_time_picker_spinner.dart';

class AddEditScreen extends StatefulWidget {
  final String? alarmId;
  final Map<String, dynamic>? alarmData;

  AddEditScreen({this.alarmId, this.alarmData});

  @override
  _AddEditScreenState createState() => _AddEditScreenState();
}

class _AddEditScreenState extends State<AddEditScreen> {
  final _formKey = GlobalKey<FormState>();
  late String _label;
  late DateTime _time;
  bool _repeat = false;
  String _sound = 'Default';
  List<bool> _selectedDays = List.filled(7, false);
  bool _enabled = true;

  @override
  void initState() {
    super.initState();
    if (widget.alarmData != null) {
      _label = widget.alarmData!['label'];
      _time = DateTime.parse(widget.alarmData!['time']);
      _repeat = widget.alarmData!['repeat'];
      _sound = widget.alarmData!['sound'] ?? 'Default';
      _selectedDays = List<bool>.from(widget.alarmData!['days'] ?? List.filled(7, false));
      _enabled = widget.alarmData!['enabled'] ?? true;
    } else {
      _label = '';
      _time = DateTime.now();
    }
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: Text(widget.alarmId == null ? 'Add Alarm' : 'Edit Alarm'),
      ),
      body: SingleChildScrollView(
        padding: EdgeInsets.only(
          bottom: MediaQuery.of(context).viewInsets.bottom,
        ),
        child: Padding(
          padding: const EdgeInsets.all(16.0),
          child: Form(
            key: _formKey,
            child: Column(
              children: [
                TextFormField(
                  initialValue: _label,
                  decoration: InputDecoration(labelText: 'Label'),
                  validator: (value) {
                    if (value == null || value.isEmpty) {
                      return 'Please enter a label';
                    }
                    return null;
                  },
                  onSaved: (value) {
                    _label = value!;
                  },
                ),
                SizedBox(height: 16.0),
                Text(
                  'Time: ${_time.hour}:${_time.minute}',
                  style: TextStyle(fontSize: 24),
                ),
                TimePickerSpinner(
                  time: _time,
                  is24HourMode: false,
                  normalTextStyle: TextStyle(
                    fontSize: 24,
                    color: Colors.black54,
                  ),
                  highlightedTextStyle: TextStyle(
                    fontSize: 24,
                    color: Colors.black,
                  ),
                  spacing: 50,
                  itemHeight: 80,
                  isForce2Digits: true,
                  onTimeChange: (time) {
                    setState(() {
                      _time = time;
                    });
                  },
                ),
                SwitchListTile(
                  title: Text('Repeat'),
                  value: _repeat,
                  onChanged: (value) {
                    setState(() {
                      _repeat = value;
                    });
                  },
                ),
                DropdownButtonFormField<String>(
                  value: _sound,
                  items: ['Default', 'Sound 1', 'Sound 2', 'Sound 3', 'Sound 4' ,'Sound 5','Sound 6','Sound 7','Sound 8','Sound 9','Sound 10'].map((sound) {

                    return DropdownMenuItem(
                      value: sound,
                      child: Text(sound),
                    );
                  }).toList(),
                  onChanged: (value) {
                    setState(() {
                      _sound = value!;
                    });
                  },
                  decoration: InputDecoration(labelText: 'Sound'),
                ),
                SizedBox(height: 16.0),
                Text(
                  'Days of the Week',
                  style: TextStyle(fontSize: 16),
                ),
                Wrap(
                  spacing: 10.0,
                  children: List.generate(7, (index) {
                    final dayLabels = ['Mon', 'Tue', 'Wed', 'Thu', 'Fri', 'Sat', 'Sun'];
                    return FilterChip(
                      label: Text(dayLabels[index]),
                      selected: _selectedDays[index],
                      onSelected: (selected) {
                        setState(() {
                          _selectedDays[index] = selected;
                        });
                      },
                    );
                  }),
                ),
                SizedBox(height: 16.0),
                ElevatedButton(
                  onPressed: () {
                    if (_formKey.currentState!.validate()) {
                      _formKey.currentState!.save();
                      final alarmId = widget.alarmId ?? DateTime.now().millisecondsSinceEpoch.toString();
                      final alarmData = {
                        'id' : alarmId,
                        'label': _label,
                        'time': _time.toIso8601String(),
                        'repeat': _repeat,
                        'sound': _sound,
                        'days': _selectedDays,
                        'enabled': _enabled,
                      };
                      Provider.of<AlarmNotifier>(context, listen: false).addOrUpdateAlarm(alarmId, alarmData);
                      Navigator.pop(context);
                    }
                  },
                  child: Text(widget.alarmId == null ? 'Add Alarm' : 'Update Alarm'),
                ),
              ],
            ),
          ),
        ),
      ),
    );
  }
}
