#pragma once

#include "IColorSchemeProvider.h"

/**
 * @brief Default color scheme provider with hardcoded dark theme colors
 *
 * This provider is used when no custom color scheme is injected.
 * It provides sensible default colors for a dark theme.
 */
class DefaultColorSchemeProvider : public IColorSchemeProvider
{
public:
    QColor contentBackgroundColor() const override { return QColor( 42,  42,  42); }
    QColor rulerBackgroundColor() const override   { return QColor( 60,  60,  60); }

    QColor gridMajorColor() const override         { return QColor( 80,  80,  80); }
    QColor gridHalfMajorColor() const override     { return QColor( 70,  70,  70); }
    QColor gridMinorColor() const override         { return QColor( 50,  50,  50); }
    QColor gridRowSeparatorColor() const override  { return QColor( 90,  90,  90); }

    QColor labelColor() const override             { return QColor(200, 200, 200); }
    QColor labelSublineColor() const override      { return QColor(150, 150, 150); }
    QColor labelTitleColor() const override        { return QColor(220, 220, 220); }
    QColor labelErrorColor() const override        { return QColor(220,  80,  80); }

    QColor connectionColor() const override        { return QColor(120, 160, 200); }
    QColor selectedColor() const override          { return QColor(100, 180, 255); }
    QColor invalidColor() const override           { return QColor(200,  80,  80); }
};