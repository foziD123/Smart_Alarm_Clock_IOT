import 'package:intl/intl.dart'; // Add this import

class Task {
  final String id;
  final String name;
  final DateTime dueDate;
  final String priority;

  Task({
    required this.id,
    required this.name,
    required this.dueDate,
    required this.priority,
  });

  Map<String, dynamic> toMap() {
    return {
      'id': id,
      'name': name,
      'dueDate': DateFormat('dd-MM-yyyy').format(dueDate), // Formatting the date
      'priority': priority,
    };
  }

  static Task fromMap(Map<String, dynamic> map) {
    return Task(
      id: map['id'] ?? '',
      name: map['name'] ?? '',
      dueDate: DateFormat('dd-MM-yyyy').parse(map['dueDate']), // Parsing the date manually
      priority: map['priority'] ?? 'Low',
    );
  }
}
