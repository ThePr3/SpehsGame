#include "Main.h"
#include "Button.h"
#include "SliderButton.h"


SliderButton::~SliderButton()
{
}
SliderButton::SliderButton(ButtonId bid, float x_pos, float y_pos, int wth, int hht, std::string str, int txtsize, sf::Font* fnt, sf::Color color, sf::Color textColor, sf::Color sliderColor, int* target) 
	: Button(bid, x_pos, y_pos, wth, hht, str, txtsize, fnt, color, textColor)
{
	originalButtonString = str;
	slider.setPosition(x_pos, y_pos);
	slider.setFillColor(sliderColor);
	sliderWidth = wth;
	slider.setSize(sf::Vector2f(sliderWidth, hht));
	targetVariable = target;
}



void SliderButton::draw(sf::RenderWindow& window, sf::Vector2i& mouse_pos)
{
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		if (mouseOverlap(mouse_pos) == true && hasFocus == true)
		{
			sliderState = (mouse_pos.x - slider.getPosition().x) / float(sliderWidth) * 100;
			sliderState = limitWithin(0, sliderState, 100);
			*targetVariable = sliderState;
		}
		else if (hasFocus == true)
			hasFocus = false;
	}
	else
		hasFocus = false;

	Button::draw(window, mouse_pos);
	slider.setSize(sf::Vector2f(int(float(sliderWidth)*(sliderState / 100.0f)), slider.getLocalBounds().height));
	text.setString(originalButtonString + " " + std::to_string(sliderState) + "%");
	setTextAlign(textAlign);

	window.draw(slider);
}

ButtonId SliderButton::checkIfPressed(sf::Vector2i& mousePos)
{
	ButtonId returnValue = Button::checkIfPressed(mousePos);
	if (returnValue != bi_false)
		hasFocus = true;
	else
		hasFocus = false;
	return returnValue;
}