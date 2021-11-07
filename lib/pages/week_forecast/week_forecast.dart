import 'package:assorted_layout_widgets/assorted_layout_widgets.dart';
import 'package:flutter/cupertino.dart';
import 'package:flutter/material.dart';
import 'package:flutter_svg/svg.dart';
import 'package:provider/provider.dart';
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
                Container(
                  width: double.infinity,
                  padding: const Pad(all: 16),
                  decoration: BoxDecoration(
                    borderRadius: BorderRadius.circular(20),
                    gradient: const LinearGradient(
                      begin: Alignment.topLeft,
                      end: Alignment.bottomRight,
                      colors: [
                        Color(0xFFCDDAF6),
                        Color(0xFF9FBEFF),
                      ],
                    ),
                  ),
                  child: Column(
                    children: [
                      Text(
                        '23 сентября',
                        style: Theme.of(context).textTheme.headline3,
                      ),
                      const SizedBox(height: 16),
                      const SizedBox(height: 16),
                      Align(
                        alignment: Alignment.bottomLeft,
                        child: Column(
                          children: [
                            buildRow(
                              Assets.icons.universal.thermometer.path,
                              '8',
                              context.select<SettingsProvider, String>(
                                  (value) => value.temperatureUnits.inString),
                            ),
                            const SizedBox(height: 24),
                            buildRow(
                              Assets.icons.universal.breeze.path,
                              '9',
                              context.select<SettingsProvider, String>(
                                  (value) => value.speedUnits.inString),
                            ),
                            const SizedBox(height: 24),
                            buildRow(
                              Assets.icons.universal.humidity.path,
                              '87',
                              '%',
                            ),
                            const SizedBox(height: 24),
                            buildRow(
                              Assets.icons.universal.barometer.path,
                              '761',
                              context.select<SettingsProvider, String>(
                                  (value) => value.pressureUnits.inString),
                            ),
                          ],
                        ),
                      ),
                    ],
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
