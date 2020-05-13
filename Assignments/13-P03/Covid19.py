
# Import and initialize the pygame library
import pygame
import random
import math
import sys

# list of colors and states for use in program
colors = ["red", "white", "grey", "blue", "pink"]
states = ["susceptible", "infected", "recovered"]


#edited starter code
im_path = "/Users/AllyMoore/Documents/GitHub/2143-OOP-Moore/Assignments/13-P03/images/"
config = {
    "sprite": {
        "width": 30,
        "height": 30,
        "speed": 1.5,
    },
    "images": {
        "black": im_path + "pac_black_30x30.png",
        "blue": im_path + "pac_blue_30x30.png",
        "green": im_path + "pac_green_30x30.png",
        "light_blue": im_path + "pac_light_blue_30x30.png",
        "orange": im_path + "pac_orange_30x30.png",
        "red": im_path + "pac_red_30x30.png",
        "white": im_path + "pac_white_30x30.png",
        "yellow": im_path + "pac_yellow_30x30.png",
        
    },
    "game": {
        "width": 1200,
        "height": 800,
        "day": 0,
        "fps": 40,
        "loop_count": 0
    },
    "sim": {
        "social_distancing": False,
        "social_distance": 20,
        "infection_radius": 10,
        "infection_rate": .20,
        "population_count": 50,
        "infected": 0,
        "recovered": 0,
        "susceptible": 0,
        "pid": 1,
    }
}


class Person(pygame.sprite.Sprite):
    #Person class 
    def __init__(self, **kwargs):
        
        # Call the parent class (Sprite) constructor
        super().__init__()
        self.width = kwargs.get("width", 10)
        self.height = kwargs.get("height", 10)
        self.speed = kwargs.get("speed", 1)
        self.coord = kwargs.get("coord", None)
        self.color = kwargs.get("color", "green")
        self.state = kwargs.get("state", "susceptible")


        # choose sprite direction
        self.dx = 0
        self.dy = 0
        while self.dx + self.dy == 0:
            self.dx = random.choice([1, -1, 0])
            self.dy = random.choice([1, -1, 0])

        # give our sprite an image and resize it
        self.image = pygame.image.load(config["images"][self.color])
        self.image = pygame.transform.scale(self.image,
                                            (self.width, self.height))

        # set sprite position
        if self.coord == None:
            self.x = int(random.random() * config["game"]["width"])
            self.y = int(random.random() * config["game"]["height"])
        else:
            self.x = self.coord[0]
            self.y = self.coord[1]

        # sprite bounding rectangle
        self.rect = self.image.get_rect(center=(self.x, self.y))
    #function to set dx and dy 
    def setDxDy(self, dx, dy):
        self.dx = dx
        self.dy = dy
    #function to access dx and dy
    def getDxDy(self):
        return (self.dx, self.dy)
    
    #function to change directed of a sprite
    def changeDirection(self, sides_contacted):
        if sides_contacted["top"]:
            self.dy = 1
        if sides_contacted["bottom"]:
            self.dy = -1
        if sides_contacted["left"]:
            self.dx = 1
        if sides_contacted["right"]:
            self.dx = -1
            
    #moves sprite and checks walls and changes direction 
    def move(self):

        sides_contacted = self.checkWalls()

        self.changeDirection(sides_contacted)

        if self.dx < 0:
            self.rect.x -= self.speed
        elif self.dx > 0:
            self.rect.x += self.speed

        if self.dy < 0:
            self.rect.y -= self.speed
        elif self.dy > 0:
            self.rect.y += self.speed
            
    #checks if sprites collided
    def checkCollide(self, other):
            sides_contacted = {
                "top": False,
                "bottom": False,
                "left": False,
                "right": False
            }
            #checks if collided and where and returns true   
            if self.rect.colliderect(other.rect):
                        
                if self.rect.top < other.rect.top:
                    sides_contacted["bottom"] = True
                    self.rect.y -= abs(self.rect.y - other.rect.y) // 2
                    

                if self.rect.left < other.rect.left:
                    sides_contacted["right"] = True
                    self.rect.x -= abs(self.rect.x - other.rect.x) // 2
                    

                if self.rect.right > other.rect.right:
                    sides_contacted["left"] = True
                    self.rect.x += abs(self.rect.x - other.rect.x) // 2
                    

                if self.rect.bottom > other.rect.bottom:
                    sides_contacted["top"] = True
                    self.rect.y += abs(self.rect.y - other.rect.y) // 2

                

                return True
            #returns false if no collision
            return False

    #checks if the sprite ha reached a wall
    def checkWalls(self):
        sides = {"top": False, "bottom": False, "left": False, "right": False}

        if self.rect.top <= 0:
            sides["top"] = True
        if self.rect.left <= 0:
            sides["left"] = True
        if self.rect.right >= config["game"]["width"]:
            sides["right"] = True
        if self.rect.bottom >= config["game"]["height"]:
            sides["bottom"] = True

        return sides


class Simulation:
    #simulation class constructor
    def __init__(self, **kwargs):
        self.population = []
        self.susceptible = []
        self.infected = []
        self.recovered = []
        self.game_width = kwargs.get("width", 200)
        self.game_height = kwargs.get("height", 200)
        self.population_count = kwargs.get("population_count", 10)
        self.sprite_group = pygame.sprite.Group()
        self.screen = kwargs.get("screen", None)
        

        print(self.screen)
        #prints an error message if theres no screen instance
        if self.screen == None:
            print(
                "Error: Simulation needs an instance of a pygame surface / screen!"
            )
            sys.exit()
    #populates sim based on population count
    def populateSim(self, pos=None):
        for _ in range(self.population_count):
            self.addPerson(state="susceptible")

    #addPerson function that is called in the populateSim function
    #and used when pressing keys to add new sprites
    def addPerson(self, **kwargs):
        width = kwargs.get("width", config["sprite"]["width"])
        height = kwargs.get("height", config["sprite"]["height"])
        speed = kwargs.get("speed", config["sprite"]["speed"])
        state = kwargs.get("state", random.choice(states))
        #assigns color based on state
        if state == "infected":
            color = kwargs.get("color", "red")
        elif state == "susceptible":
            color = kwargs.get("color", "white")
        elif state == "recovered":
            color = kwargs.get("color", "blue")

         
       
        #assigns random coordinate to draw sprite   
        x = random.randint(0, self.game_width)
        y = random.randint(0, self.game_height)
        coord = kwargs.get("coord", [x, y])
        #passes variables to Person class constructor
        p = Person(width=config["sprite"]["width"],
                   height=config["sprite"]["height"],
                   speed=config["sprite"]["speed"],
                   coord=coord,
                   color=color,
                   state=state)
        #adds person to correct list
        if p.state == "infected":
            self.infected.append(p)    
        elif p.state == "recovered":
            self.recovered.append(p)    
        elif p.state == "susceptible":
            self.susceptible.append(p)
        #adds every new person to population and sprite_group   
        self.population.append(p)
            
        self.sprite_group.add(p)

        
        
    def simRun(self,**kwargs):

        
        # loop through each person and call a move method
        for i in range(len(self.population)):
            self.population[i].move()
            for j in range(len(self.population)):
                if self.population[i] != self.population[j]:
                    # getting x,y for both sprites
                    x1, y1 = self.population[i].rect.center
                    x2, y2 = self.population[j].rect.center

                    # distance
                    d = math.sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2))
                    #assigns collided and true or false value
                    collided = Person.checkCollide(self.population[i],self.population[j])
                    #checks if collided is true
                    if collided:
                        #checks if the sprites state is infected
                        if self.population[i].state == "infected":
                            #changes susceptible to infected if it comes in contact with the
                            #infected sprite
                            if self.population[j].state == "susceptible":
                                if d < self.population[i].width*1 and random.random() < 0.2:
        
                                    self.population[j].image = pygame.image.load(config["images"]["red"])
                                    self.population[j].image = pygame.transform.scale(self.population[j].image,
                                                (self.population[j].width, self.population[j].height))
                                    self.population[j].state = "infected"
                                    self.infected.append(self.population[j])
                                    
                                    self.susceptible.pop()
                            #changes infected sprite to recovered if it comes in contact with a recovered sprite
                            elif self.population[j].state == "recovered":
                               
                                self.population[i].image = pygame.image.load(config["images"]["blue"])
                                self.population[i].image = pygame.transform.scale(self.population[i].image,
                                            (self.population[i].width, self.population[i].height))
                                self.population[i].state = "recovered"
                                self.recovered.append(self.population[i])
                                self.infected.pop()
                        dx,dy = self.population[j].getDxDy()
                        self.population[j].setDxDy(dx * -1, dy * -1)

        self.sprite_group.draw(self.screen)


#__________________________________________________________________________

if __name__ == '__main__':
    pygame.init()




    # define the RGB value for white, 
    #  green, blue colour . 
    white = (255, 255, 255) 
    green = (0, 255, 0) 
    blue = (0, 0, 128)
    grey = (25, 25, 25) 
    
    # sets up the display surface of dimensions X and Y 
    X = 300
    Y = 300
    display_surface = pygame.display.set_mode((X, Y )) 
    
    #sets windiw name
    pygame.display.set_caption('Covid Simulation') 
    
    #sets up the font and text size
    font = pygame.font.Font('freesansbold.ttf', 20) 




    # Sets up the window
    screen = pygame.display.set_mode([1200,860])

    
    sim = Simulation(screen=screen,
                     width=config["game"]["width"],
                     height=config["game"]["height"],
                     population_count=config["sim"]["population_count"],
                     susceptible=config["sim"]["susceptible"],
                     infected=config["sim"]["infected"],
                     recovered=config["sim"]["recovered"])

    sim.populateSim()

    # keeps game loop running at specific frames per second
    clock = pygame.time.Clock()

    # Run until the user asks to quit
    running = True


    #___ GAME LOOP ____________________________________________________________
    while running:
        # Fill the background with blackish
        # Do not do this after you draw sprites!
        screen.fill((30, 30, 30))
        
         

        #this code block sets up the text on the bottom right of the screen
        #it updates as the lists are edited
        text = font.render('There are ' + str(len(sim.susceptible)) + " Susceptible, " + str(len(sim.infected)) + ' Infected, ' + str(len(sim.recovered)) + ' Recovered, and ' + str(len(sim.population)) + ' Total', True, white, grey)  
        textIm = text.get_rect()   
        textIm.right = config["game"]["width"]
        textIm.bottom = config["game"]["height"] 
        display_surface.blit(text, textIm)

        keys=pygame.key.get_pressed
        # Did the user click the window close button?
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                running = False

            # handles the mouse interaction, left button adds recovered, right adds infected
            if event.type == pygame.MOUSEBUTTONUP:
                if event.button == 1:               
                    pos = pygame.mouse.get_pos()    

                    sim.addPerson(coord=pos,state="recovered")
                    
                elif event.button == 3:             
                    pos = pygame.mouse.get_pos()
                    sim.addPerson(coord=pos,state="infected")
            #handles keyboard interaction, spacebar adds susceptible
            if event.type == pygame.KEYDOWN:
                if event.key == pygame.K_SPACE:
                    sim.addPerson(state="susceptible")                    
                    
                

        #___CONTROL SIMULATION HERE_____________________________________________________________

        sim.simRun()

        #___END CONTROL SIMULATION_____________________________________________________________

        #This keeps game loop running at a constant frames per second
        clock.tick(config["game"]["fps"]) 

        #Count number of loops game runs
        config["game"]["loop_count"] += 1

        #Refresh screen
        pygame.display.flip()

#___ END GAME LOOP ____________________________________________________________
# Done! Time to quit.
    pygame.quit()
