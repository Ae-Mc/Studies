import 'package:auto_route/auto_route.dart';
import 'package:weather_app/pages/about/about.dart';
import 'package:weather_app/pages/home/home.dart';
import 'package:weather_app/pages/search/search.dart';
import 'package:weather_app/pages/settings/settings.dart';
import 'package:weather_app/pages/week_forecast/week_forecast.dart';

@MaterialAutoRouter(
  replaceInRouteName: 'Page,Route',
  routes: <AutoRoute>[
    AutoRoute(page: HomePage),
    AutoRoute(page: SettingsPage),
    AutoRoute(page: AboutPage),
    AutoRoute(page: WeekForecastPage),
    AutoRoute(page: SearchPage),
  ],
)
class $AppRouter {}
