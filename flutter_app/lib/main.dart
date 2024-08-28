import 'package:flutter/material.dart';
import 'package:firebase_core/firebase_core.dart';
import 'package:provider/provider.dart';
import 'package:tmp_flutter/notifiers/alarm_notifier.dart';
import 'package:tmp_flutter/notifiers/task_notifier.dart';
import 'package:tmp_flutter/notifiers/theme_notifier.dart'; // Import the ThemeNotifier
import 'package:tmp_flutter/screens/splash_screen.dart';
import 'package:tmp_flutter/screens/home_screen.dart';
import 'package:tmp_flutter/screens/settings_screen.dart';
import 'package:tmp_flutter/screens/sync_screen.dart';
import 'package:tmp_flutter/screens/task_list_screen.dart';
import 'package:tmp_flutter/screens/add_edit_screen.dart';
import 'package:tmp_flutter/screens/task_edit_screen.dart';
import 'package:tmp_flutter/notifiers/esp32_status_notifier.dart';

void main() async {
  WidgetsFlutterBinding.ensureInitialized();
  await Firebase.initializeApp();
  runApp(MyApp());
}

class MyApp extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return MultiProvider(
      providers: [
        ChangeNotifierProvider(create: (_) => AlarmNotifier()),
        ChangeNotifierProvider(create: (_) => TaskNotifier()),
        ChangeNotifierProvider(create: (_) => ThemeNotifier()), // Add ThemeNotifier
        ChangeNotifierProvider(create: (_) => ESP32StatusNotifier()),
      ],
      child: Consumer<ThemeNotifier>(
        builder: (context, themeNotifier, child) {
          return MaterialApp(
            title: 'Smart Alarm Clock',
            theme: themeNotifier.currentTheme,
            initialRoute: '/',
            routes: {
              '/': (context) => SplashScreen(),
              '/home': (context) => HomeScreen(),
              '/settings': (context) => SettingsScreen(),
              '/sync': (context) => SyncScreen(),
              '/addEdit': (context) => AddEditScreen(),
              '/taskEdit': (context) => TaskEditScreen(),
              '/tasks': (context) => TaskListScreen(),
            },
          );
        },
      ),
    );
  }
}
