import 'package:assorted_layout_widgets/assorted_layout_widgets.dart';
import 'package:carousel_slider/carousel_slider.dart';
import 'package:flutter/cupertino.dart';
import 'package:flutter/material.dart';
import 'package:flutter_svg/svg.dart';
import 'package:intl/intl.dart';
import 'package:provider/provider.dart';
import 'package:weather_app/data/models/day_weather.dart';
import 'package:weather_app/data/providers/providers.dart';
import 'package:weather_app/data/providers/settings_provider.dart';
import 'package:weather_app/gen/assets.gen.dart';

class WeekForecastPage extends StatelessWidget {
  const WeekForecastPage({Key? key}) : super(key: key);

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      body: SafeArea(
        child: Padding(
          padding: const Pad(vertical: 25, horizontal: 20),
          child: SizedBox.expand(
            child: Column(
              children: [
                Text(
                  'Прогноз на неделю',
                  style: Theme.of(context).textTheme.headline3,
                ),
                const SizedBox(height: 32),
                Expanded(
                  child: OverflowBox(
                    maxWidth: MediaQuery.of(context).size.width,
                    child: CarouselSlider.builder(
                      itemCount: context.select<WeatherProvider, int>(
                        (value) => value.currentWeather.daily.length,
                      ),
                      itemBuilder: (context, index, _) =>
                          Builder(builder: (context) {
                        final dayWeather =
                            context.select<WeatherProvider, DayWeather>(
                          (value) {
                            return value.currentWeather.daily[index];
                          },
                        );
                        return WeatherCard(dayWeather);
                      }),
                      options: CarouselOptions(
                        enableInfiniteScroll: false,
                        viewportFraction: 0.9,
                        enlargeCenterPage: true,
                        height: double.infinity,
                      ),
                    ),
                  ),
                ),
                const SizedBox(height: 40),
                OutlinedButton(
                  onPressed: () => Navigator.of(context).pop(),
                  child: const Text('Назад на главную'),
                  style: ButtonStyle(
                    side: MaterialStateProperty.all(BorderSide(
                        color: Theme.of(context).colorScheme.onBackground)),
                    foregroundColor: MaterialStateProperty.all(
                        Theme.of(context).colorScheme.onBackground),
                  ),
                ),
              ],
            ),
          ),
        ),
      ),
    );
  }
}

class WeatherCard extends StatelessWidget {
  final DayWeather dayWeather;
  const WeatherCard(this.dayWeather, {Key? key}) : super(key: key);

  @override
  Widget build(BuildContext context) {
    return Align(
      alignment: Alignment.topCenter,
      child: Container(
        width: double.infinity,
        padding: const Pad(all: 16),
        decoration: BoxDecoration(
          borderRadius: BorderRadius.circular(20),
          gradient: LinearGradient(
            begin: Alignment.topLeft,
            end: Alignment.bottomRight,
            colors: Theme.of(context).brightness == Brightness.light
                ? [
                    const Color(0xFFCDDAF6),
                    const Color(0xFF9FBEFF),
                  ]
                : [
                    const Color(0xFF213A70),
                    const Color(0xFF102042),
                  ],
          ),
        ),
        child: Column(
          crossAxisAlignment: CrossAxisAlignment.start,
          mainAxisSize: MainAxisSize.min,
          children: [
            Text(
              DateFormat('d MMMM', 'ru_RU').format(dayWeather.dt),
              style: Theme.of(context).textTheme.headline3,
            ),
            const SizedBox(height: 16),
            WeatherProvider.getWeatherIconAssetFromWeatherDataIcon(
              dayWeather.weather.first.icon,
            ).image(height: 85),
            const SizedBox(height: 35),
            Align(
              alignment: Alignment.bottomLeft,
              child: Column(
                children: [
                  buildRow(
                    Assets.icons.universal.thermometer.path,
                    context.select<WeatherProvider, String>(
                      (value) => value
                          .getTempInCurrentUnits(
                              (dayWeather.temp.min + dayWeather.temp.max) / 2)
                          .round()
                          .toString(),
                    ),
                    context.select<SettingsProvider, String>(
                        (value) => value.temperatureUnits.inString),
                  ),
                  const SizedBox(height: 24),
                  buildRow(
                    Assets.icons.universal.breeze.path,
                    context.select<WeatherProvider, String>(
                      (value) => value
                          .getSpeedInCurrentUnits(dayWeather.windSpeed)
                          .round()
                          .toString(),
                    ),
                    context.select<SettingsProvider, String>(
                        (value) => value.speedUnits.inString),
                  ),
                  const SizedBox(height: 24),
                  buildRow(
                    Assets.icons.universal.humidity.path,
                    dayWeather.humidity.toString(),
                    '%',
                  ),
                  const SizedBox(height: 24),
                  buildRow(
                    Assets.icons.universal.barometer.path,
                    context.select<WeatherProvider, String>(
                      (value) => value
                          .getPressureInCurrentUnits(dayWeather.pressure)
                          .round()
                          .toString(),
                    ),
                    context.select<SettingsProvider, String>(
                        (value) => value.pressureUnits.inString),
                  ),
                ],
              ),
            ),
          ],
        ),
      ),
    );
  }

  Builder buildRow(String iconAsset, String text1, String text2) {
    return Builder(builder: (context) {
      return Row(
        children: [
          SvgPicture.asset(
            iconAsset,
            color: Theme.of(context).colorScheme.onSurface,
            width: 24,
            height: 24,
          ),
          const SizedBox(width: 12),
          RichText(
            text: TextSpan(
              children: [
                TextSpan(text: text1),
                TextSpan(
                  text: text2,
                  style: TextStyle(
                    color: Theme.of(context).colorScheme.onSurface,
                  ),
                ),
              ],
              style: Theme.of(context).textTheme.headline5,
            ),
          ),
        ],
      );
    });
  }
}
