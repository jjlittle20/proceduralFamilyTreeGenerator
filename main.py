from names import first_male_names, first_female_names, british_surnames
import random
import itertools
import time
import sys
import math


class Humanoid:
    def __init__(self) -> None:
        pass


class Relationship:
    def __init__(self, recipientId) -> None:
        self.length = 0
        self.recipientId = recipientId


class Lover(Relationship):
    def __init__(self, recipient) -> None:
        super().__init__(recipient)


class Partner(Relationship):
    def __init__(self, recipient) -> None:
        super().__init__(recipient)


class Married(Relationship):
    def __init__(self, recipient) -> None:
        super().__init__(recipient)


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
        self.married = Married("")
        self.partner = Partner("")
        self.lover = Lover("")
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
            "loverID: ",
            "None" if self.lover.recipientId is None else self.lover.recipientId,
            "\n",
            "loverName: ",
            "None"
            if self.lover.recipientId is None
            else entities[self.lover.recipientId].name,
            "\n",
            "lover time: ",
            self.lover.length,
            "\n",
            "pregnant: ",
            self.isPregnant,
            "\n",
            "pregnant cycle: ",
            self.currentPregnancyLength,
            "\n",
            "mother: ",
            self.parents[0]
            if type(self.parents[0]) == str
            else entities[self.parents[0]].name,
            "\n",
            "father: ",
            self.parents[1]
            if type(self.parents[1]) == str
            else entities[self.parents[1]].name,
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
            name[1] = entities[self.parents[1]].name[1]

        return name

    def pregnancyCheck(self):

        if self.gender == "m":
            return

        elif (
            self.age[0] >= self.breedAge
            and not self.isPregnant
            and self.lover.recipientId is not None
        ):

            pregnancyChance = random.uniform(0.15, 0.25)

            if random.random() < pregnancyChance:

                self.isPregnant = True
                self.currentPregnancyLength = 0
        elif self.currentPregnancyLength >= self.maxPregnancyLength:
            self.isPregnant = False
            self.currentPregnancyLength = 0

            self.giveBirth([self.id, self.lover.recipientId])
        elif self.isPregnant:
            self.currentPregnancyLength = self.currentPregnancyLength + 1

    def deathCheck(self):
        if not self.isDead and self.age[0] >= self.maxAge:
            if self.isPregnant and self.currentPregnancyLength >= 8:
                self.giveBirth([self.id, self.lover.recipientId])
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
            # self.getStats()

    def giveBirth(self, parents):

        newHuman = Human(False, parents, False, False)
        entities[newHuman.id] = newHuman

    def relationshipCheck(self):

        if self.lover.recipientId != "":

            self.lover.length = self.lover.length + 1
        if self.partner.recipientId != "":
            self.partner.length = self.partner.length + 1
        if self.married.recipientId != "":
            self.married.length = self.married.length + 1
        if self.lover.recipientId == "" and self.age[0] >= self.breedAge:

            gender = self.gender

            if gender == "m" and availableLoversF:

                choice = random.choice(availableLoversF)

                self.lover.recipientId = entities[choice].id
            elif gender == "f" and availableLoversM:

                choice = random.choice(availableLoversM)

                self.lover.recipientId = entities[choice].id


def cycle():
    global availableLoversM
    global availableLoversF
    availableLoversM = []
    availableLoversF = []
    start_time = time.time()
    availableLoversM = [
        (key)
        for (key, value) in entities.items()
        if entities[key].gender == "m" and entities[key].lover.recipientId == ""
    ]
    availableLoversF = [
        (key)
        for (key, value) in entities.items()
        if entities[key].gender == "f" and entities[key].lover.recipientId == ""
    ]

    x = time.time() - start_time

    times.append(round(x, 2))

    entitiesCopy = entities.copy()
    for key in entitiesCopy:
        entitiesCopy[key].singleCycle()


def mainLoop():

    start_time = time.time()
    seedAmount = 20
    months = 1200
    global times
    times = []
    global entities
    entities = {}

    for seed in range(seedAmount):
        seedM = Human("m", "seed", True, True)
        seedF = Human("f", "seed", True, True)
        seedM.partner.recipientId = seedF.id
        seedF.partner.recipientId = seedM.id
        seedM.lover.recipientId = seedF.id
        seedF.lover.recipientId = seedM.id

        entities[seedM.id] = seedM
        entities[seedF.id] = seedF
        seedF.name[1] = entities[seedF.partner.recipientId].name[1]
    for x in range(months):
        print("cycle: ", x)
        cycle()
    print("--- %s seconds ---" % (time.time() - start_time))
    living = [(key) for (key) in entities if entities[key].isDead == False]
    # dead = [(key) for (key) in entities if entities.isDead == True]
    print("seed amount: ", seedAmount)
    print("months: ", months)
    print("years: ", months / 12)
    print("living: ", len(living))
    print("dead: ", len(entities) - len(living))
    print("total: ", len(entities))
    if len(times) > 0:

        print("times total: ", sum(times))
    entitiesSizeOf = sys.getsizeof(entities)
    print("size of entities(system: BYTES): ", entitiesSizeOf)
    print("size of entities(system: KILOBYTES): ", entitiesSizeOf / 1000)
    print("size of entities(system: MEGABYTES): ", entitiesSizeOf / 1000000)


mainLoop()
