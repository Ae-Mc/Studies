import 'package:assorted_layout_widgets/assorted_layout_widgets.dart';
import 'package:flutter/cupertino.dart';
import 'package:flutter/material.dart';
import 'package:flutter_neumorphic/flutter_neumorphic.dart';
import 'package:provider/provider.dart';
import 'package:weather_app/data/models/place.dart';
import 'package:weather_app/data/providers/settings_provider.dart';
import 'package:weather_app/data/providers/weather_provider.dart';
import 'package:weather_app/data/storage/storage.dart';
import 'package:weather_app/widgets/page_header.dart';

class FavoritesPage extends StatefulWidget {
  const FavoritesPage({Key? key}) : super(key: key);

  @override
  State<FavoritesPage> createState() => _FavoritesPageState();
}

class _FavoritesPageState extends State<FavoritesPage> {
  @override
  Widget build(BuildContext context) {
    return Scaffold(
      body: SafeArea(
        child: Column(
          crossAxisAlignment: CrossAxisAlignment.stretch,
          children: [
            const Padding(
              padding: Pad(horizontal: 20, top: 32, bottom: 16),
              child: PageHeader(title: 'Избранное'),
            ),
            Expanded(
              child: ListView.separated(
                padding: const Pad(horizontal: 20, vertical: 16),
                itemCount: Storage.favorites.length,
                itemBuilder: (_, index) =>
                    _favoriteRow(Storage.favorites[index]),
                separatorBuilder: (_, __) => const SizedBox(height: 16),
              ),
            ),
          ],
        ),
      ),
    );
  }

  Widget _favoriteRow(Place place) {
    return Builder(builder: (context) {
      return InkWell(
        borderRadius: BorderRadius.circular(25),
        onTap: () async {
          context.read<SettingsProvider>().activePlace = place;
          await context.read<WeatherProvider>().updateWeatherData();
          Navigator.of(context).pop();
        },
        child: Stack(
          children: [
            SizedBox(
              width: double.infinity,
              child: Neumorphic(
                style: NeumorphicStyle(
                  color: Theme.of(context).colorScheme.surface,
                  depth: Theme.of(context).colorScheme.brightness ==
                          Brightness.light
                      ? -4
                      : 3,
                  lightSource: LightSource.top,
                  boxShape:
                      NeumorphicBoxShape.roundRect(BorderRadius.circular(15)),
                ),
                child: Padding(
                  padding: const Pad(all: 16),
                  child: Text(
                      place.country == null
                          ? place.name
                          : '${place.country}, ${place.name}',
                      style: Theme.of(context).textTheme.overline),
                ),
              ),
            ),
            Positioned(
              top: 0,
              bottom: 0,
              right: 0,
              child: AspectRatio(
                aspectRatio: 1,
                child: GestureDetector(
                  onTap: () {
                    Storage.removeFavorite(place)
                        .then((_) => setState(() => {}));
                  },
                  child: Neumorphic(
                    style: NeumorphicStyle(
                      color: Theme.of(context).colorScheme.brightness ==
                              Brightness.light
                          ? const Color(0xFFC8DAFF)
                          : const Color(0xFF152A53),
                      depth: Theme.of(context).colorScheme.brightness ==
                              Brightness.light
                          ? 4
                          : 0,
                      boxShape: NeumorphicBoxShape.roundRect(
                          BorderRadius.circular(15)),
                    ),
                    child: Icon(
                      Icons.close,
                      color: Theme.of(context).colorScheme.onBackground,
                    ),
                  ),
                ),
              ),
            ),
          ],
        ),
      );
    });
  }
}
