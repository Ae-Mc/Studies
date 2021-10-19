import 'package:flutter/material.dart';
import 'package:provider/provider.dart';
import 'package:weather_app/data/providers/settings_provider.dart';
import 'package:weather_app/router/router.gr.dart';
import 'package:weather_app/theme/style.dart';

class App extends StatelessWidget {
  App({Key? key}) : super(key: key);

  final _appRouter = AppRouter();

  // This widget is the root of your application.
  @override
  Widget build(BuildContext context) {
    return MultiProvider(
      providers: [
        ChangeNotifierProvider(
          create: (_) => SettingsProvider(),
        )
      ],
      builder: (context, _) {
        final style = Style();
        return MaterialApp.router(
          title: 'Flutter Demo',
          theme: style.lightTheme,
          darkTheme: style.darkTheme,
          themeMode: Provider.of<SettingsProvider>(context).themeMode,
          routerDelegate: _appRouter.delegate(),
          routeInformationParser: _appRouter.defaultRouteParser(),
        );
      }),
    );
  }
}
