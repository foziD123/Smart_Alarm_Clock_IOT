import 'package:flutter/material.dart';
import 'package:provider/provider.dart';
import 'package:tmp_flutter/notifiers/task_notifier.dart';
import 'package:tmp_flutter/task_model.dart';

class TaskEditScreen extends StatefulWidget {
  final Task? task;

  TaskEditScreen({this.task});

  @override
  _TaskEditScreenState createState() => _TaskEditScreenState();
}

class _TaskEditScreenState extends State<TaskEditScreen> {
  final _formKey = GlobalKey<FormState>();
  late String _name;
  late DateTime _dueDate;
  late String _priority;

  @override
  void initState() {
    super.initState();
    if (widget.task != null) {
      _name = widget.task!.name;
      _dueDate = widget.task!.dueDate;
      _priority = widget.task!.priority;
    } else {
      _name = '';
      _dueDate = DateTime.now();
      _priority = 'Low';
    }
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: Text(widget.task == null ? 'Add Task' : 'Edit Task'),
      ),
      body: Padding(
        padding: const EdgeInsets.all(16.0),
        child: Form(
          key: _formKey,
          child: Column(
            children: [
              TextFormField(
                initialValue: _name,
                decoration: InputDecoration(labelText: 'Task Name'),
                validator: (value) {
                  if (value == null || value.isEmpty) {
                    return 'Please enter a task name';
                  }
                  return null;
                },
                onSaved: (value) {
                  _name = value!;
                },
              ),
              SizedBox(height: 16.0),
              ListTile(
                title: Text('Due Date: ${_dueDate.toLocal()}'.split(' ')[0]),
                trailing: Icon(Icons.calendar_today),
                onTap: _pickDueDate,
              ),
              DropdownButtonFormField<String>(
                value: _priority,
                items: ['Low', 'Medium', 'High'].map((priority) {
                  return DropdownMenuItem(
                    value: priority,
                    child: Text(priority),
                  );
                }).toList(),
                onChanged: (value) {
                  setState(() {
                    _priority = value!;
                  });
                },
                decoration: InputDecoration(labelText: 'Priority'),
              ),
              SizedBox(height: 16.0),
              ElevatedButton(
                onPressed: _saveTask,
                child: Text(widget.task == null ? 'Add Task' : 'Update Task'),
              ),
            ],
          ),
        ),
      ),
    );
  }

  void _pickDueDate() async {
    DateTime? pickedDate = await showDatePicker(
      context: context,
      initialDate: _dueDate,
      firstDate: DateTime.now(),
      lastDate: DateTime(2101),
    );

    if (pickedDate != null && pickedDate != _dueDate) {
      setState(() {
        _dueDate = pickedDate;
      });
    }
  }

  void _saveTask() {
    if (_formKey.currentState!.validate()) {
      _formKey.currentState!.save();

      final newTask = Task(
        id: widget.task?.id ?? DateTime.now().millisecondsSinceEpoch.toString(),
        name: _name,
        dueDate: _dueDate,
        priority: _priority,
      );

      Provider.of<TaskNotifier>(context, listen: false).addOrUpdateTask(newTask);
      Navigator.pop(context);
    }
  }
}
