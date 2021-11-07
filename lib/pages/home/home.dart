import 'package:animated_size_and_fade/animated_size_and_fade.dart';
import 'package:assorted_layout_widgets/assorted_layout_widgets.dart';
import 'package:auto_route/auto_route.dart';
import 'package:flutter/cupertino.dart';
import 'package:flutter/material.dart';
import 'package:flutter/rendering.dart';
import 'package:flutter/widgets.dart';
import 'package:flutter_neumorphic/flutter_neumorphic.dart';
import 'package:flutter_svg/flutter_svg.dart';
import 'package:intl/intl.dart';
import 'package:provider/provider.dart';
import 'package:weather_app/data/providers/settings_provider.dart';
import 'package:weather_app/data/providers/weather_provider.dart';
import 'package:weather_app/gen/assets.gen.dart';
import 'package:weather_app/pages/home/widgets/custom_bottom_sheet.dart';
import 'package:weather_app/router/router.gr.dart';
import 'package:weather_app/widgets/my_icon_button.dart';

class HomePage extends StatefulWidget {
  const HomePage({Key? key}) : super(key: key);

  @override
  State<HomePage> createState() => _HomePageState();
}

class _HomePageState extends State<HomePage> {
  var bottomSheetExpanded = false;

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      body: Stack(
        children: [
          _header(),
          Align(
            alignment: Alignment.bottomCenter,
            child: _bottomSheet(),
          ),
        ],
      ),
      drawer: _drawer(context),
    );
  }

  Widget _header() {
    return Builder(builder: (context) {
      return Ink(
        decoration: BoxDecoration(
          image: DecorationImage(
            image: (Theme.of(context).brightness == Brightness.light
                ? Assets.images.background
                : Assets.images.darkBackground),
            fit: BoxFit.cover,
          ),
        ),
        child: SafeArea(
          child: Column(
            mainAxisAlignment: MainAxisAlignment.start,
            crossAxisAlignment: CrossAxisAlignment.stretch,
            children: [
              Padding(
                padding: const Pad(horizontal: 25, top: 25),
                child: Row(
                  mainAxisAlignment: MainAxisAlignment.spaceBetween,
                  crossAxisAlignment: CrossAxisAlignment.start,
                  children: [
                    MyIconButton(
                      onTap: () => Scaffold.of(context).openDrawer(),
                      icon: const Icon(Icons.menu_rounded),
                    ),
                    Expanded(
                      child: Column(
                        mainAxisSize: MainAxisSize.min,
                        children: [
                          AnimatedSizeAndFade(
                            sizeDuration: const Duration(milliseconds: 300),
                            fadeDuration: const Duration(milliseconds: 300),
                            child: bottomSheetExpanded
                                ? Align(
                                    alignment: Alignment.topCenter,
                                    child: Padding(
                                      padding: const Pad(top: 8, bottom: 19),
                                      child: Text(
                                        context.select<SettingsProvider,
                                                String>(
                                            (value) => value.activePlace.name),
                                        style: Theme.of(context)
                                            .textTheme
                                            .headline5
                                            ?.copyWith(
                                              color: Theme.of(context)
                                                  .iconTheme
                                                  .color,
                                            ),
                                      ),
                                    ),
                                  )
                                : const SizedBox(width: double.infinity),
                          ),
                          Text.rich(
                            TextSpan(
                              children: [
                                TextSpan(
                                  text: context.select<WeatherProvider, String>(
                                    (value) => value
                                        .getTempInCurrentUnits(
                                            value.currentWeather.current.temp)
                                        .toStringAsFixed(1),
                                  ),
                                ),
                                TextSpan(
                                  text:
                                      context.select<SettingsProvider, String>(
                                    (value) => value.temperatureUnits.inString,
                                  ),
                                  style: Theme.of(context)
                                      .textTheme
                                      .headline1
                                      ?.copyWith(
                                        color:
                                            Theme.of(context).iconTheme.color,
                                        letterSpacing: -11.5,
                                      ),
                                ),
                              ],
                              style: Theme.of(context)
                                  .textTheme
                                  .headline1
                                  ?.copyWith(
                                    color: Theme.of(context).iconTheme.color,
                                  ),
                            ),
                          ),
                          const SizedBox(height: 8),
                          AnimatedSizeAndFade(
                            sizeDuration: const Duration(milliseconds: 300),
                            fadeDuration: const Duration(milliseconds: 300),
                            child: bottomSheetExpanded
                                ? const SizedBox(width: double.infinity)
                                : Align(
                                    alignment: Alignment.topCenter,
                                    child: Text(
                                      DateFormat.yMMMd('ru_RU')
                                          .format(DateTime.now().toLocal()),
                                      style: Theme.of(context)
                                          .textTheme
                                          .headline2
                                          ?.copyWith(
                                            color: Theme.of(context)
                                                .iconTheme
                                                .color,
                                          ),
                                    ),
                                  ),
                          ),
                        ],
                      ),
                    ),
                    MyIconButton(
                      onTap: () =>
                          AutoRouter.of(context).push(const SearchRoute()),
                      icon: const Icon(
                        CupertinoIcons.plus_circle,
                      ),
                    ),
                  ],
                ),
              ),
            ],
          ),
        ),
      );
    });
  }

  Drawer _drawer(BuildContext context) {
    return Drawer(
      child: ListTileTheme(
        iconColor: Theme.of(context).colorScheme.onBackground,
        style: ListTileStyle.drawer,
        child: SafeArea(
          child: Padding(
            padding: const Pad(horizontal: 20, top: 30),
            child: ListView(
              children: [
                Text(
                  'Weather app',
                  style: Theme.of(context).textTheme.headline2,
                ),
                ListTile(
                  onTap: () =>
                      AutoRouter.of(context).push(const SettingsRoute()),
                  leading: const Icon(Icons.settings_outlined),
                  title: const Text('Настройки'),
                ),
                ListTile(
                  onTap: () =>
                      AutoRouter.of(context).push(const FavoritesRoute()),
                  leading: const Icon(Icons.favorite_border_rounded),
                  title: const Text('Избранные'),
                ),
                ListTile(
                  onTap: () => AutoRouter.of(context).push(const AboutRoute()),
                  leading: const Icon(Icons.account_circle_outlined),
                  title: const Text('О приложении'),
                ),
              ],
            ),
          ),
        ),
      ),
    );
  }

  Widget _bottomSheet() {
    return CustomBottomSheet(
      builder: _bottomSheetBuilder,
      onStateChange: (extended) =>
          setState(() => bottomSheetExpanded = extended),
      minHeight: 250,
      maxHeight: 450,
    );
  }

  Widget _bottomSheetBuilder(BuildContext context, bool extended) {
    return Container(
      padding: const Pad(horizontal: 20),
      decoration: BoxDecoration(
        color: Theme.of(context).colorScheme.background,
        borderRadius: const BorderRadius.only(
          topLeft: Radius.circular(15),
          topRight: Radius.circular(15),
        ),
      ),
      child: OverflowBox(
        alignment: Alignment.topCenter,
        maxHeight: double.infinity,
        child: Column(
          children: [
            Container(
              padding: const Pad(vertical: 14.5),
              color: Colors.transparent,
              child: Container(
                decoration: BoxDecoration(
                  color: Theme.of(context).colorScheme.primaryVariant,
                  borderRadius: BorderRadius.circular(20),
                ),
                width: 60,
                height: 3,
              ),
            ),
            AnimatedSizeAndFade.showHide(
              show: extended,
              child: Align(
                alignment: Alignment.topCenter,
                child: Padding(
                  padding: const Pad(bottom: 32),
                  child: Text(
                    DateFormat('d MMMM', 'ru_RU')
                        .format(DateTime.now().toLocal()),
                  ),
                ),
              ),
              sizeDuration: const Duration(milliseconds: 300),
              fadeDuration: const Duration(milliseconds: 300),
            ),
            cardsRow(),
            const SizedBox(height: 16),
            AnimatedSizeAndFade.showHide(
              show: !extended,
              child: OutlinedButton(
                onPressed: () => AutoRouter.of(context).push(
                  const WeekForecastRoute(),
                ),
                child: const Text(
                  'Прогноз на неделю',
                ),
                style: ButtonStyle(
                  foregroundColor: MaterialStateProperty.all(
                    Theme.of(context).colorScheme.primaryVariant,
                  ),
                  side: MaterialStateProperty.all(BorderSide(
                    color: Theme.of(context).colorScheme.primaryVariant,
                  )),
                ),
              ),
              sizeDuration: const Duration(milliseconds: 300),
              fadeDuration: const Duration(milliseconds: 300),
            ),
            AnimatedSize(
              alignment: Alignment.topCenter,
              child: extended
                  ? Padding(
                      padding: const Pad(top: 16),
                      child: weatherIndicators(context),
                    )
                  : const SizedBox(width: double.infinity),
              duration: const Duration(milliseconds: 300),
            ),
          ],
        ),
      ),
    );
  }

  Widget cardsRow() {
    return Builder(builder: (context) {
      return Row(
        mainAxisAlignment: MainAxisAlignment.spaceBetween,
        children: [
          timeCard(
            '06:00',
            cardIcon(Assets.icons.light.lightning.svg()),
            getTempOnHour(6),
          ),
          timeCard(
            '12:00',
            cardIcon(Assets.icons.light.sun.svg()),
            getTempOnHour(12),
          ),
          timeCard(
            '18:00',
            cardIcon(Assets.icons.light.rain3Drops.svg()),
            getTempOnHour(18),
          ),
          timeCard(
            '00:00',
            cardIcon(Assets.icons.light.rain.svg()),
            getTempOnHour(0),
          ),
        ],
      );
    });
  }

  Widget timeCard(
    String time,
    Widget icon,
    String temperature,
  ) {
    return Builder(builder: (context) {
      return ConstrainedBox(
        constraints: const BoxConstraints(minWidth: 65, minHeight: 122),
        child: Neumorphic(
          padding: const Pad(horizontal: 9, vertical: 5),
          style: NeumorphicStyle(
            depth: 3,
            color: Theme.of(context).colorScheme.surface,
          ),
          child: Column(
            mainAxisSize: MainAxisSize.min,
            children: [
              Text(time),
              const SizedBox(height: 11),
              SizedBox(height: 40, width: 40, child: icon),
              const SizedBox(height: 13),
              Text(temperature),
            ],
          ),
        ),
      );
    });
  }

  String getTempOnHour(int hourNum) {
    final weatherProvider = context.read<WeatherProvider>();
    return weatherProvider
            .getTempInCurrentUnits(weatherProvider.currentWeather.hourly
                .getRange(0, 24)
                .where((element) => element.dt.hour == hourNum)
                .first
                .temp)
            .round()
            .toString() +
        context.read<SettingsProvider>().temperatureUnits.inString;
  }

  Widget cardIcon(Widget icon) {
    return LayoutBuilder(builder: (context, constraints) {
      return Container(
        decoration: BoxDecoration(
          gradient: RadialGradient(
            colors: [
              Theme.of(context).brightness == Brightness.light
                  ? Colors.white
                  : Colors.white.withOpacity(0.01),
              Theme.of(context).colorScheme.surface,
            ],
          ),
        ),
        child: icon,
      );
    });
  }

  Widget weatherIndicators(BuildContext context) {
    return Table(
      defaultColumnWidth: const FlexColumnWidth(1),
      columnWidths: const {
        1: FixedColumnWidth(20),
      },
      children: [
        TableRow(
          children: [
            TableCell(
              child: weatherIndicatorCard(
                Assets.icons.universal.thermometer.path,
                context.select<WeatherProvider, String>(
                  (value) => value
                      .getTempInCurrentUnits(value.currentWeather.current.temp)
                      .toStringAsFixed(1),
                ),
                context.select<SettingsProvider, String>(
                    (value) => value.temperatureUnits.inString),
              ),
            ),
            const SizedBox(),
            TableCell(
              child: weatherIndicatorCard(
                Assets.icons.universal.humidity.path,
                context.select<WeatherProvider, String>(
                  (value) => value.currentWeather.current.humidity.toString(),
                ),
                '%',
              ),
            ),
          ],
        ),
        TableRow(
          children: [
            TableCell(
              child: Padding(
                padding: const Pad(top: 8),
                child: weatherIndicatorCard(
                  Assets.icons.universal.breeze.path,
                  context.select<WeatherProvider, String>(
                    (value) => value
                        .getSpeedInCurrentUnits(
                            value.currentWeather.current.windSpeed)
                        .toStringAsFixed(1),
                  ),
                  context.select<SettingsProvider, String>(
                      (value) => value.speedUnits.inString),
                ),
              ),
            ),
            const SizedBox(),
            TableCell(
              child: Padding(
                padding: const Pad(top: 8),
                child: weatherIndicatorCard(
                  Assets.icons.universal.barometer.path,
                  context.select<WeatherProvider, String>((value) => value
                      .getPressureInCurrentUnits(
                          value.currentWeather.current.pressure)
                      .toStringAsFixed(0)),
                  context.select<SettingsProvider, String>(
                      (value) => value.pressureUnits.inString),
                ),
              ),
            ),
          ],
        ),
      ],
    );
  }

  Widget weatherIndicatorCard(String iconAsset, String text1, String text2) {
    return Builder(
      builder: (context) {
        return Neumorphic(
          style: NeumorphicStyle(color: Theme.of(context).colorScheme.surface),
          child: Padding(
            padding: const Pad(vertical: 20, horizontal: 8),
            child: Center(
              child: Row(
                mainAxisAlignment: MainAxisAlignment.center,
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
              ),
            ),
          ),
        );
      },
    );
  }
}
