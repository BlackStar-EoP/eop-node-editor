#include "EditorColorScheme.h"
#include "IColorSchemeProvider.h"
#include "DefaultColorSchemeProvider.h"

// Static member initialization
IColorSchemeProvider* EditorColorScheme::provider_ = nullptr;
DefaultColorSchemeProvider* EditorColorScheme::defaultProvider_ = nullptr;

void EditorColorScheme::setProvider(IColorSchemeProvider* provider)
{
	provider_ = provider;
}

void EditorColorScheme::notifyColorsChanged()
{
	emit instance().colorsChanged();
}

EditorColorScheme& EditorColorScheme::instance()
{
	static EditorColorScheme instance;
	return instance;
}

IColorSchemeProvider* EditorColorScheme::getProvider()
{
	if (provider_ != nullptr)
		return provider_;

	// Lazy-create default provider
	if (defaultProvider_ == nullptr)
		defaultProvider_ = new DefaultColorSchemeProvider();

	return defaultProvider_;
}

QColor EditorColorScheme::contentBackgroundColor()
{
	return getProvider()->contentBackgroundColor();
}

QColor EditorColorScheme::rulerBackgroundColor()
{
	return getProvider()->rulerBackgroundColor();
}

QColor EditorColorScheme::gridMajorColor()
{
	return getProvider()->gridMajorColor();
}

QColor EditorColorScheme::gridHalfMajorColor()
{
	return getProvider()->gridHalfMajorColor();
}

QColor EditorColorScheme::gridMinorColor()
{
	return getProvider()->gridMinorColor();
}

QColor EditorColorScheme::gridRowQSeperatorColor()
{
	return getProvider()->gridRowSeparatorColor();
}

QColor EditorColorScheme::labelColor()
{
	return getProvider()->labelColor();
}

QColor EditorColorScheme::labelSublineColor()
{
	return getProvider()->labelSublineColor();
}

QColor EditorColorScheme::labelTitleColor()
{
	return getProvider()->labelTitleColor();
}

QColor EditorColorScheme::labelErrorColor()
{
	return getProvider()->labelErrorColor();
}

QColor EditorColorScheme::connection_color()
{
	return getProvider()->connectionColor();
}

QColor EditorColorScheme::selectedColor()
{
	return getProvider()->selectedColor();
}

QColor EditorColorScheme::invalidColor()
{
	return getProvider()->invalidColor();
}