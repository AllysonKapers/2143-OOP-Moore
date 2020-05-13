# Covid-19 Simulation

### Description:
- This program uses Pygame to model the spread of Covid-19. It has 2 classes, a Person Class and a Population Class. It infects "people" randomly if a susceptible person comes in contact with an infected person and uses the infection rate of 0.2 to determine whether or not a suscpetible person will be infected. If an infected person comes in contact with a recovered person, they move to being recovered. 

### Controls:
- Left Mouse Button: Adds recovered psprite
- Right Mouse Button: Adds infected sprite
- Space Bar: Adds susceptible sprite

### Required Packages:
- Pygame

### Update Starting Population:
- Under sim, in the config code block, you can change the starting population but changing the number following "population_count". 