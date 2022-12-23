from names import first_male_names, first_female_names, british_surnames
import random
import itertools


class Humanoid:
    def __init__(self) -> None:
        pass


class Human(Humanoid):
    id_iter = itertools.count()

    def __init__(self, gender, parents, isMarried=False, isSeed=False) -> None:
        super().__init__()
        self.id = next(self.id_iter)
        self.maxAge = 45
        self.breedAge = 18
        self.maxPregnancyLength = 12
        self.currentPregnancyLength = 0
        self.isSeed = isSeed
        self.parents = self.getParents(parents)
        self.gender = self.getGender(gender)
        self.age = self.getAge()
        self.name = self.generateName()
        self.isPregnant = False
        self.isMarried = isMarried
        self.partner = ""
        self.partnerLength = 0
        self.lover = ""
        self.loverLength = 0
        self.isDead = False
        self.maidenName = self.name

    def getStats(self):

        print(
            "id: ",
            self.id,
            "\n",
            "name: ",
            self.name[0],
            self.name[1],
            "\n",
            "dead: ",
            self.isDead,
            "\n",
            "gender: ",
            self.gender,
            "\n",
            "age: ",
            self.age,
            "\n",
            "lover: ",
            "None" if type(self.lover) == str else self.lover.name,
            "\n",
            "pregnant: ",
            self.isPregnant,
            "\n",
            "pregnant cycle: ",
            self.currentPregnancyLength,
            "\n",
            "mother: ",
            self.parents[0] if type(self.parents[0]) == str else self.parents[0].name,
            "\n",
            "father: ",
            self.parents[1] if type(self.parents[1]) == str else self.parents[1].name,
            "\n",
        )

    def getParents(self, parents):
        if parents == "seed":
            return ["Seed Mother", "Seed Father"]
        else:
            return parents

    def getGender(self, gender):
        if self.isSeed:
            return gender
        else:
            return random.choice(["m", "f"])

    def getAge(self):
        if self.isSeed:
            return [random.randint(self.breedAge, self.breedAge + 10), 0]
        else:
            return [0, 0]

    def generateName(self):
        name = ["", ""]

        if self.gender == "m":
            name[0] = random.choice(first_male_names)
        else:
            name[0] = random.choice(first_female_names)

        if self.isSeed:
            name[1] = random.choice(british_surnames).upper()

        else:
            name[1] = self.parents[1].name[1]

        return name

    def pregnancyCheck(self):

        if self.gender == "m":
            return

        elif self.age[0] >= self.breedAge and not self.isPregnant and self.lover:
            pregnancyChance = random.uniform(0.15, 0.25)

            if random.random() < pregnancyChance:

                self.isPregnant = True
                self.currentPregnancyLength = 0
        elif self.currentPregnancyLength >= self.maxPregnancyLength:
            self.isPregnant = False
            self.currentPregnancyLength = 0
            self.giveBirth([self, self.lover])
        elif self.isPregnant:
            self.currentPregnancyLength = self.currentPregnancyLength + 1

    def deathCheck(self):
        if not self.isDead and self.age[0] >= self.maxAge:
            if self.isPregnant and self.currentPregnancyLength >= 8:
                self.giveBirth([self, self.lover])
            self.isDead = True
        else:
            if self.age[1] >= 12:
                self.age[0] = self.age[0] + 1
                self.age[1] = 0
            else:
                self.age[1] = self.age[1] + 1

    def singleCycle(self):
        if self.isDead:
            return
        else:
            self.relationshipCheck()
            self.pregnancyCheck()
            self.deathCheck()
            self.getStats()

    def giveBirth(self, parents):
        newHuman = Human(False, parents, False, False)
        entities.append(newHuman)

    def relationshipCheck(self):
        if self.lover == "" and self.age[0] >= self.breedAge:
            gender = "m" if self.gender == "f" else "f"
            availableLovers = [
                entity
                for entity in entities
                if entity.gender == gender and entity.lover == ""
            ]
            if availableLovers:
                self.lover = random.choice(availableLovers)
        # elif self.partner == "" and self.loverLength == 4:
        #     self.partner == self.lover


def cycle():

    for x in entities:
        x.singleCycle()


def mainLoop():

    global entities
    entities = []
    seedM = Human("m", "seed", True, True)
    seedF = Human("f", "seed", True, True)
    seedM.partner = seedF
    seedF.partner = seedM
    seedM.lover = seedF
    seedF.lover = seedM
    seedF.name[1] = seedF.partner.name[1]
    entities.append(seedM)
    entities.append(seedF)
    for x in range(600):
        print("cycle: ", x)
        cycle()


mainLoop()
