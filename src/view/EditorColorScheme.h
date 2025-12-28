#pragma once

#include <QColor>
#include <QObject>

class IColorSchemeProvider;
class DefaultColorSchemeProvider;

/**
 * @brief Color scheme manager for the node editor
 *
 * This class provides color values for the node editor UI.
 * By default, it uses hardcoded colors from DefaultColorSchemeProvider.
 * A custom IColorSchemeProvider can be injected via setProvider() to
 * integrate with external theme systems.
 */
class EditorColorScheme : public QObject
{
	Q_OBJECT

public:
	/**
	 * @brief Inject a custom color scheme provider
	 * @param provider Pointer to provider, or nullptr to use defaults
	 *
	 * The provider is NOT owned by EditorColorScheme - caller must
	 * ensure it remains valid for the lifetime of its use.
	 */
	static void setProvider(IColorSchemeProvider* provider);

	/**
	 * @brief Notify that colors have changed
	 *
	 * Call this to trigger a repaint of all node editor views when
	 * the color scheme has been updated. Emits the colorsChanged() signal.
	 */
	static void notifyColorsChanged();

	/**
	 * @brief Get the singleton instance
	 */
	static EditorColorScheme& instance();

	// Color accessors
	static QColor contentBackgroundColor();
	static QColor rulerBackgroundColor();

	static QColor gridMajorColor();
	static QColor gridHalfMajorColor();
	static QColor gridMinorColor();

	static QColor gridRowQSeperatorColor();

	static QColor labelColor();
	static QColor labelSublineColor();
	static QColor labelTitleColor();
	static QColor labelErrorColor();

	static QColor connection_color();
	static QColor selectedColor();
	static QColor invalidColor();

signals:
	/**
	 * @brief Emitted when colors have changed
	 *
	 * Connect to this signal to repaint views when the color scheme updates.
	 */
	void colorsChanged();

private:
	EditorColorScheme() = default;

	static IColorSchemeProvider* getProvider();

	static IColorSchemeProvider* provider_;
	static DefaultColorSchemeProvider* defaultProvider_;
};
