#pragma once

#include <QColor>

/**
 * @brief Abstract interface for providing color scheme values to the node editor
 *
 * This interface allows the node editor to be independent of external theme systems.
 * Implement this interface to provide custom colors, or use the DefaultColorSchemeProvider
 * for hardcoded defaults.
 */
class IColorSchemeProvider
{
public:
    virtual ~IColorSchemeProvider() = default;

    // Background colors
    virtual QColor contentBackgroundColor() const = 0;
    virtual QColor rulerBackgroundColor() const = 0;

    // Grid colors
    virtual QColor gridMajorColor() const = 0;
    virtual QColor gridHalfMajorColor() const = 0;
    virtual QColor gridMinorColor() const = 0;
    virtual QColor gridRowSeparatorColor() const = 0;

    // Label colors
    virtual QColor labelColor() const = 0;
    virtual QColor labelSublineColor() const = 0;
    virtual QColor labelTitleColor() const = 0;
    virtual QColor labelErrorColor() const = 0;

    // Node editor specific colors
    virtual QColor connectionColor() const = 0;
    virtual QColor selectedColor() const = 0;
    virtual QColor invalidColor() const = 0;
};