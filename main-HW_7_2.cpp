#define CPPHTTPLIB_OPENSSL_SUPPORT

#include <SFML/Graphics.hpp>
#include <iostream>
#include <nlohmann/json.hpp>
#include <httplib.h>
#include <ctime>
//https://api.covid19api.com/total/country/taiwan?from=2021-06-25T00:00:00Z&to=2021-06-26T00:00:00Z
int main() {
	time_t curr_time;
	struct tm newtime;
	curr_time = time(NULL);
	localtime_s(&newtime,&curr_time);
	// https://api.covid19api.com/world/total
	httplib::SSLClient client("api.covid19api.com");	

	// JSON
	const std::string data = client.Get("/world/total")->body;

	auto j = nlohmann::json::parse(data);

	int confirmed = j["TotalConfirmed"];
	int death = j["TotalDeaths"];
	int recovered = j["TotalRecovered"];
	//int timestamp = j["Date"];
	sf::Font font;   // arial.ttf	
	if (!font.loadFromFile("arial.ttf")) {
		return EXIT_FAILURE;
	}

	sf::String confirmedString(std::to_string(confirmed));
	sf::Text confirmedText(confirmedString, font);
	confirmedText.setPosition(200, 20);
	
	sf::String deathString(std::to_string(death));
	sf::Text deathText(deathString, font);
	deathText.setPosition(200, 80);

	sf::String recoveredString(std::to_string(recovered));
	sf::Text recoveredText(recoveredString, font);
	recoveredText.setPosition(200, 140);

	sf::String yearString("Last time Update:" 
		+std::to_string(1900+newtime.tm_year)+"/"
		+ std::to_string(1 + newtime.tm_mon) + "/" 
		+ std::to_string(newtime.tm_mday) + " "
		+ std::to_string(newtime.tm_hour) + ":"
		+ std::to_string(newtime.tm_min) + ":"
		+ std::to_string(newtime.tm_sec));
	sf::Text yeartext(yearString,font,20U);
	yeartext.setPosition(50, 200);

	sf::RenderWindow window(sf::VideoMode(400, 240), L"COVID-19");	
	sf::Texture confirmedTexture;
	if (!confirmedTexture.loadFromFile("confirmed.png")) {
		return EXIT_FAILURE;
	}
	sf::Texture deathTexture;
	if (!deathTexture.loadFromFile("death.png")) {
		return EXIT_FAILURE;
	}
	sf::Texture recoveredTexture;
	if (!recoveredTexture.loadFromFile("recovered.png")) {
		return EXIT_FAILURE;
	}

	sf::Sprite confirmedSprite(confirmedTexture);
	confirmedSprite.setPosition(15, 20);

	sf::Sprite deathSprite(deathTexture);
	deathSprite.setPosition(15, 80);

	sf::Sprite recoveredSprite(recoveredTexture);
	recoveredSprite.setPosition(15, 140);

	window.setFramerateLimit(30);
	while (window.isOpen()) {
		sf::Event evt;
		if (window.pollEvent(evt)) {
			if (evt.type == sf::Event::Closed) {
				window.close();
			}
		}

		window.clear();
		window.draw(confirmedSprite);
		window.draw(deathSprite);
		window.draw(recoveredSprite);
		window.draw(confirmedText);
		window.draw(deathText);
		window.draw(recoveredText);
		window.draw(yeartext);
		window.display();		
	}
	return EXIT_SUCCESS;
}