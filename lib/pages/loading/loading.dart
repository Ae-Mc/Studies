import 'package:auto_route/auto_route.dart';
import 'package:flutter/material.dart';
import 'package:intl/date_symbol_data_local.dart';
import 'package:provider/provider.dart';
import 'package:weather_app/data/providers/weather_provider.dart';
import 'package:weather_app/data/storage/storage.dart';
import 'package:weather_app/router/router.gr.dart';

class LoadingPage extends StatelessWidget {
  const LoadingPage({Key? key}) : super(key: key);

  Future<void> futures(BuildContext context) async {
    var router = AutoRouter.of(context);
    await Future.wait([
      Storage.initialize().then((value) =>
          Provider.of<WeatherProvider>(context, listen: false).initialize()),
      initializeDateFormatting('ru_RU'),
    ]);
    router.replace(const HomeRoute());
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      body: SafeArea(
        child: FutureBuilder(
            future: futures(context),
            builder: (context, snapshot) {
              return SizedBox.expand(
                child: Column(
                  children: [
                    const Spacer(),
                    Text('Weather',
                        style: Theme.of(context).textTheme.headline2),
                    const Spacer(),
                    CircularProgressIndicator(
                      color: Theme.of(context).colorScheme.onBackground,
                    ),
                    const Spacer(),
                    const Spacer(),
                  ],
                ),
              );
            }),
      ),
    );
  }
}
