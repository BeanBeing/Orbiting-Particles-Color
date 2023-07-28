#include <SFML/Graphics.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <vector>
#include <sstream>
#include <cmath>

// The source that has Gravitational pull
class GravitySource
{
   sf::Vector2f m_pos;
   float m_strength;
   sf::CircleShape m_Shape;
   
public:
   GravitySource(float pos_x, float pos_y, float strength) : m_pos{pos_x, pos_y}, m_strength{strength}
   {
      m_Shape.setPosition(m_pos);
      m_Shape.setRadius(12);
   }

   /**--SETTERS--**/
   void set_pos(float pos_x, float pos_y)
   {
      m_pos.x = pos_x;
      m_pos.y = pos_y;
   }

   void set_strength(float newStrength)
   {
      m_strength = newStrength;
   }
   /**--END OF SETTERS--**/

   /**--GETTERS--**/
   sf::Vector2f get_pos() const
   {
      return m_pos;
   }

   float get_strength() const
   {
      return m_strength;
   }
   /**--END OF GETTERS--**/

   void render(sf::RenderWindow &window)
   {
      window.draw(m_Shape);
   }
};

// The source that latches onto that gravitational pull
class Particles
{
   sf::Vector2f m_pos;		// Get position of particle
   sf::Vector2f m_velocity;	// velocity of particle
   sf::CircleShape m_Shape;	// Shape of the particle being pulled
   
public:
   Particles(float pos_x, float pos_y, float vel_x, float vel_y) : m_pos{pos_x, pos_y}, m_velocity{vel_x, vel_y}
   {
      m_Shape.setPosition(m_pos);
      m_Shape.setRadius(6);
   }

   // Manually change the color of a Particle
   void setShapeColor(sf::Color color)
   {
      m_Shape.setFillColor(color);
   }

   // Sets the shapePosition and Velocity. Required for resetting its state
   void setShapePos_vel(float pos_x, float pos_y, float vel_x, float vel_y)
   {
      m_pos.x = pos_x;
      m_pos.y = pos_y;
      m_velocity.x = vel_x;
      m_velocity.y = vel_y;
      m_Shape.setPosition(m_pos);
   }

   // Pythagorean theorem to determine the distance
   // between the GravitySource and the particle
   void update_physics(const GravitySource &obj)
   {
      float distance_x = obj.get_pos().x - m_pos.x;
      float distance_y = obj.get_pos().y - m_pos.y;

      // Gets the distance between obj and particle
      float distance = sqrt((distance_x * distance_x) + (distance_y * distance_y));

      // Any number multiply by a division of 1 divided by a number
      // is that number.
      // EX: 1/8 = 0.125 ; 5/8 = 0.125 * 5
      float inverse_distance = 1.f / distance;

      // Multiplication is faster than division
      float normalized_x = inverse_distance * distance_x;
      float normalized_y = inverse_distance * distance_y;

      // 1 / x * 1/x formula for inverse_square_law
      // inverse_distance provides mathematical equation of that formula
      float inverse_square_law = inverse_distance * inverse_distance;

      // Allows for the object to be affected based on distance for x and y
      float acceleration_x = normalized_x * obj.get_strength() * inverse_square_law;
      float acceleration_y = normalized_y * obj.get_strength() * inverse_square_law;

      // velocity adds with acceleration
      m_velocity.x += acceleration_x;
      m_velocity.y += acceleration_y;

      // add position based on velocity
      m_pos.x += m_velocity.x;
      m_pos.y += m_velocity.y;

      // Update the position
      m_Shape.setPosition(m_pos);
   }

   // Display the shape
   void render(sf::RenderWindow &window)
   {
      window.draw(m_Shape);
   }

};

// Transition between 3 colors from 0 to 1
sf::Color map_val_to_color(float value) // value is 0 - 1
{
   // If its less than 0, be 0
   if(value < 0.0f)
   {
      value = 0.0f;
   } else if(value > 1.0f){	// If its more than 1.0f, keep it as 1.0f
      value = 1.0f;
   }

   unsigned char r = 0, g = 0, b = 0;

   // Interpolate between blue green and red
   // 0 - 0.5	 interpolate blue to green
   // 0.5 - 1.0f intepolate green to red

   if(value < 0.5f)
   {
      b = 255 * (1.0f - 2 * value);	// First layer is 1.0f
      g = 255 * 2 * value;
   } else {
      g = 255 * (2.0f - 2 * value);	// Second layer increment it by 1
      r = 255 * (2 * value - 1);
   }

   return sf::Color(r, g, b);
   
}
int main()
{
   sf::RenderWindow window(sf::VideoMode(1200, 800), "Particled-Lemniscate");
   window.setFramerateLimit(144);

   /**--Texts--**/
   sf::Font font;
   font.loadFromFile("/usr/share/fonts/noto/NotoSans-Black.ttf");

   sf::Text fpsText;
   fpsText.setFont(font);
   fpsText.setFillColor(sf::Color::White);
   fpsText.setCharacterSize(30);
   fpsText.setPosition(10, 20);

   sf::Text controlsText;
   controlsText.setFont(font);
   controlsText.setFillColor(sf::Color::White);
   controlsText.setCharacterSize(20);
   controlsText.setPosition(340, 20);
   controlsText.setString("For colors press UP-Arrow | To de-color press DOWN-Arrow");
   
   /**--END-Texts--**/

   std::vector<GravitySource> sources;

   sources.push_back(GravitySource(200, 400, 3000));
   sources.push_back(GravitySource(1000, 400,4000));
   sources.push_back(GravitySource(600, 400, 5000));

   // The amount of particles to generate
   std::size_t num_particles { 4000 };

   std::vector<Particles> particles;

   // The amount of particles to add onto the vector
   for(std::size_t i = 0; i < num_particles; i++)
   {
      // Pushes a particle with the set position of x, y, and x_velocity.
      // Differs the y velocity  based on a range of 0.2 up to 0.3
      particles.push_back(Particles(900, 700, 2, 0.2 + (0.1 / num_particles) * i));
      
   }

   // For FPS
   int lastTime { 0 };
   sf::Clock clock;
      
   while(window.isOpen())
   {
      sf::Event event;

      while(window.pollEvent(event))
      {
         if(event.type == sf::Event::Closed){
            window.close();
         }

         if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)){
            window.close();
         }

         /**--APPLIES COLOR TO THE PARTICLES--**/
         if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
         {
            for(std::size_t i{ 0 } ; i < num_particles; i++)
            {
               // Val will be scaled from 0 to 1
               float val = static_cast<float>(i) / num_particles;

               sf::Color changed_color = map_val_to_color(val);

               particles[i].setShapeColor(changed_color);
            }
         }

         if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
         {
            for(std::size_t i{ 0 } ; i < num_particles; i++)
            {
               particles[i].setShapeColor(sf::Color::White);
            }
         }
         /**--------------------------------**/
      }

      // FPS
      sf::Time dt = clock.restart();
      lastTime++;
      if(lastTime == 10)	// Speed at which it'll update
      {
         std::stringstream fps;
         fps << "FPS: " << 1 / dt.asSeconds();
         fpsText.setString(fps.str());
         lastTime = 0;
      }

      window.clear();
      window.draw(fpsText);
      window.draw(controlsText);

      // Update the physics for each particle
      // By the amount of Gravtional Sources
      for(std::size_t i = 0; i < sources.size(); i++)
      {
         for(std::size_t j = 0; j < particles.size(); j++)
         {
            particles[j].update_physics(sources[i]);
         }
      }
      
      /**--DRAW--**/

      for(std::size_t i = 0; i < sources.size(); i++)
      {
         sources[i].render(window);
      }
      
      for(std::size_t j = 0; j < particles.size(); j++)
      {
            
         particles[j].render(window);
      }

      window.display();
   }
   
   return 0;
}
