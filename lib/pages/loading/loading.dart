import 'package:auto_route/auto_route.dart';
import 'package:dio/dio.dart';
import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import 'package:internet_connection_checker/internet_connection_checker.dart';
import 'package:intl/date_symbol_data_local.dart';
import 'package:shared_preferences/shared_preferences.dart';
import 'package:weather_app/core/network/network_info.dart';
import 'package:weather_app/features/settings/data/datasources/settings_local_data_source_impl.dart';
import 'package:weather_app/features/settings/data/repositories/settings_repository_impl.dart';
import 'package:weather_app/features/settings/presentation/bloc/settings_bloc.dart';
import 'package:weather_app/features/weather/data/datasources/weather_remote_data_source_impl.dart';
import 'package:weather_app/features/weather/data/repositories/weather_repository_impl.dart';
import 'package:weather_app/features/weather/presentation/bloc/weather_bloc.dart';
import 'package:weather_app/router/router.gr.dart';

class LoadingPage extends StatelessWidget {
  const LoadingPage({Key? key}) : super(key: key);

  Future<void> init(BuildContext context) async {
    BlocProvider.of<SettingsBloc>(context).add(
      SettingsInitialized(
        SettingsRepositoryImpl(
          localDataSource: SettingsLocalDataSourceImpl(
            await SharedPreferences.getInstance(),
          ),
        ),
      ),
    );

    BlocProvider.of<WeatherBloc>(context).add(
      WeatherInitialized(
        WeatherRepositoryImpl(
          networkInfo: NetworkInfoImpl(InternetConnectionChecker()),
          remoteDataSource: WeatherRemoteDataSourceImpl(
            api: OpenWeatherApi(Dio()),
          ),
        ),
        BlocProvider.of<SettingsBloc>(context),
      ),
    );

    var router = AutoRouter.of(context);
    await Future.wait([
      initializeDateFormatting('ru_RU'),
      BlocProvider.of<SettingsBloc>(context)
          .stream
          .firstWhere((element) => element is SettingsSuccess)
          .then((value) => BlocProvider.of<WeatherBloc>(context)
              .add(const WeatherUpdateRequested()))
    ]);
    router.replace(const HomeRoute());
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      body: SafeArea(
        child: FutureBuilder(
          future: init(context)..onError((error, stackTrace) => throw error!),
          builder: (context, snapshot) {
            return SizedBox.expand(
              child: Column(
                children: [
                  const Spacer(),
                  Text('Weather', style: Theme.of(context).textTheme.headline2),
                  const Spacer(),
                  CircularProgressIndicator(
                    color: Theme.of(context).colorScheme.onBackground,
                  ),
                  const Spacer(),
                  const Spacer(),
                ],
              ),
            );
          },
        ),
      ),
    );
  }
}
