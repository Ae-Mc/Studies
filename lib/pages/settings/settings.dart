import 'package:assorted_layout_widgets/assorted_layout_widgets.dart';
import 'package:flutter/material.dart' hide Switch;
import 'package:provider/provider.dart';
import 'package:weather_app/data/models/settings.dart';
import 'package:weather_app/data/providers/settings_provider.dart';
import 'package:weather_app/pages/settings/widgets/switch.dart';
import 'package:weather_app/widgets/page_header.dart';

class SettingsPage extends StatefulWidget {
  const SettingsPage({Key? key}) : super(key: key);

  @override
  State<SettingsPage> createState() => _SettingsPageState();
}

class _SettingsPageState extends State<SettingsPage> {
  final _animationDuration = const Duration(milliseconds: 300);

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      body: SafeArea(
        child: Padding(
          padding: const Pad(horizontal: 20, top: 32),
          child: Column(
            mainAxisSize: MainAxisSize.max,
            crossAxisAlignment: CrossAxisAlignment.start,
            children: [
              const PageHeader(title: 'Настройки'),
              const SizedBox(height: 32),
              Text(
                'Единицы измерения',
                style: Theme.of(context).textTheme.caption,
              ),
              const SizedBox(height: 16),
              IntrinsicHeight(
                child: OverflowBox(
                  maxWidth: MediaQuery.of(context).size.width,
                  child: Ink(
                    decoration: BoxDecoration(
                        color: Theme.of(context).scaffoldBackgroundColor,
                        borderRadius: BorderRadius.circular(30),
                        boxShadow: [
                          BoxShadow(
                            color: Colors.white.withOpacity(0.25),
                            blurRadius: 4,
                            offset: const Offset(0, -4),
                          ),
                          BoxShadow(
                            color: Colors.black.withOpacity(0.1),
                            blurRadius: 10,
                            offset: const Offset(0, 4),
                          ),
                        ]),
                    child: Padding(
                      padding: const Pad(horizontal: 20),
                      child: Consumer<SettingsProvider>(
                        builder: (context, state, _) => Column(
                          mainAxisSize: MainAxisSize.min,
                          crossAxisAlignment: CrossAxisAlignment.stretch,
                          children: [
                            _switchRow(
                              context: context,
                              title: 'Температура',
                              text1: '˚C',
                              text2: '˚F',
                              state: state.temperatureUnits ==
                                  TemperatureUnits.farenheit,
                              onTap: state.switchTemperatureUnits,
                            ),
                            _divider(),
                            _switchRow(
                              context: context,
                              title: 'Сила ветра',
                              text1: 'м/с',
                              text2: 'км/ч',
                              state: state.speedUnits ==
                                  SpeedUnits.kilometersPerHour,
                              onTap: state.switchSpeedUnits,
                            ),
                            _divider(),
                            _switchRow(
                              context: context,
                              title: 'Давление',
                              text1: 'мм.рт.ст',
                              text2: 'гПа',
                              state: state.pressureUnits ==
                                  PressureUnits.gigaPascal,
                              onTap: state.switchPressureUnits,
                            ),
                          ],
                        ),
                      ),
                    ),
                  ),
                ),
              ),
              const SizedBox(height: 32),
              Text(
                'Интерфейс',
                style: Theme.of(context).textTheme.caption,
              ),
              const SizedBox(height: 16),
              _switchRow(
                context: context,
                title: 'Тема',
                text1: 'Тёмная',
                text2: 'Светлая',
                state: Provider.of<SettingsProvider>(context).themeMode ==
                    ThemeMode.light,
                onTap: Provider.of<SettingsProvider>(context).switchTheme,
              ),
            ],
          ),
        ),
      ),
    );
  }

  Widget _switchRow(
      {required BuildContext context,
      required String title,
      required String text1,
      required String text2,
      required bool state,
      required void Function() onTap}) {
    return InkWell(
      onTap: onTap,
      child: Container(
        decoration: const BoxDecoration(shape: BoxShape.circle),
        padding: const Pad(vertical: 13),
        child: Row(
          mainAxisAlignment: MainAxisAlignment.spaceBetween,
          children: [
            Text(title, style: Theme.of(context).textTheme.subtitle1),
            Switch(
              state: state,
              text1: text1,
              text2: text2,
              duration: _animationDuration,
            ),
          ],
        ),
      ),
    );
  }

  Widget _divider() {
    return const Divider(height: 1);
  }
}
