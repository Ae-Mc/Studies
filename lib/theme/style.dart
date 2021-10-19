import 'package:assorted_layout_widgets/assorted_layout_widgets.dart';
import 'package:flex_color_scheme/flex_color_scheme.dart';
import 'package:flutter/material.dart';
import 'package:google_fonts/google_fonts.dart';

class Style {
  late final TextTheme textTheme;
  late final ThemeData lightTheme;
  late final ThemeData darkTheme;

  Style() {
    textTheme = GoogleFonts.manropeTextTheme(
      const TextTheme(
        bodyText1: TextStyle(fontSize: 18, fontWeight: FontWeight.w500),
        bodyText2: TextStyle(fontSize: 17),
        headline1: TextStyle(
          fontSize: 70,
          fontWeight: FontWeight.w800,
          letterSpacing: -0.15 * 70,
        ),
        headline2: TextStyle(fontSize: 35, fontWeight: FontWeight.w600),
        headline3: TextStyle(fontSize: 24, fontWeight: FontWeight.w600),
        headline4: TextStyle(fontSize: 20, fontWeight: FontWeight.w600),
        headline5: TextStyle(
          fontSize: 16,
          fontWeight: FontWeight.w600,
          letterSpacing: -0.15,
        ),
        overline: TextStyle(
          fontSize: 13,
          fontWeight: FontWeight.w600,
          letterSpacing: 1,
        ),
        subtitle1: TextStyle(fontSize: 14, fontWeight: FontWeight.w600),
        subtitle2: TextStyle(fontSize: 12, fontWeight: FontWeight.w600),
        caption: TextStyle(fontSize: 10, fontWeight: FontWeight.w600),
        button: TextStyle(
          fontSize: 14,
          fontWeight: FontWeight.w500,
          letterSpacing: 0,
        ),
      ),
    );

    lightTheme = FlexColorScheme.light(
      colors: const FlexSchemeColor(
        primary: Color(0xFF0256FF),
        primaryVariant: Color(0xFF038CFE),
        secondary: Color(0xFF4B5F88),
        secondaryVariant: Color(0xFF4B5F88),
      ),
      background: const Color(0xFFE2EBFF),
      surface: const Color(0xFFE0E9FD),
      onSurface: const Color(0xFF5A5A5A),
      scaffoldBackground: const Color(0xFFE2EBFF),
      visualDensity: VisualDensity.compact,
    ).toTheme.copyWith(
          textTheme: textTheme
              .apply(
                displayColor: Colors.black,
                bodyColor: Colors.black,
              )
              .merge(
                const TextTheme(
                  headline1: TextStyle(color: Colors.white, inherit: true),
                  caption: TextStyle(color: Color(0xFF828282), inherit: true),
                ),
              ),
          dividerTheme: DividerThemeData(
            color: Colors.black.withOpacity(0.15),
            thickness: 1,
          ),
          iconTheme: const IconThemeData(color: Colors.white, size: 24),
          inputDecorationTheme: InputDecorationTheme(
            hintStyle:
                textTheme.overline?.copyWith(color: const Color(0xFF828282)),
            border: InputBorder.none,
            contentPadding: const Pad(vertical: 16),
          ),
          outlinedButtonTheme: OutlinedButtonThemeData(
            style: ButtonStyle(
              padding: MaterialStateProperty.all(const Pad(all: 8)),
              shape: MaterialStateProperty.all(
                RoundedRectangleBorder(
                  borderRadius: BorderRadius.circular(10),
                ),
              ),
            ),
          ),
          materialTapTargetSize: MaterialTapTargetSize.shrinkWrap,
        );

    darkTheme = FlexColorScheme.dark(
      colors: const FlexSchemeColor(
        primary: Color(0xFF051340),
        primaryVariant: Colors.white,
        secondary: Color(0xFFEEF4FF),
        secondaryVariant: Color(0xFFEEF4FF),
      ),
      background: const Color(0xFF0D172B),
      surface: const Color(0xFF0E182C),
      onSurface: const Color(0xFFB1B1B1),
      scaffoldBackground: const Color(0xFF0D172B),
      visualDensity: VisualDensity.compact,
    ).toTheme.copyWith(
          textTheme: textTheme
              .apply(
                displayColor: Colors.white,
                bodyColor: Colors.white,
              )
              .merge(
                const TextTheme(
                  caption: TextStyle(color: Color(0xFFAAAAAA), inherit: true),
                ),
              ),
          dividerTheme: DividerThemeData(
            color: Colors.white.withOpacity(0.15),
            thickness: 1,
          ),
          iconTheme: const IconThemeData(color: Colors.white, size: 24),
          inputDecorationTheme: InputDecorationTheme(
            hintStyle:
                textTheme.overline?.copyWith(color: const Color(0xFFD4D4D4)),
            border: InputBorder.none,
            contentPadding: const Pad(vertical: 16),
          ),
          outlinedButtonTheme: OutlinedButtonThemeData(
            style: ButtonStyle(
              padding: MaterialStateProperty.all(const Pad(all: 8)),
              shape: MaterialStateProperty.all(
                RoundedRectangleBorder(
                  borderRadius: BorderRadius.circular(10),
                ),
              ),
            ),
          ),
          materialTapTargetSize: MaterialTapTargetSize.shrinkWrap,
        );
  }
}
