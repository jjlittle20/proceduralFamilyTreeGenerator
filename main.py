from names import first_male_names, first_female_names, british_surnames
import random
import itertools
import time
import sys
import math
import pickle
import os

professions = {
    "farmer": {"name": "farmer"},
    "woodcutter": {"name": "woodcutter"},
    "carpenter": {"name": "carpenter"},
    "miner": {"name": "miner"},
    "mason": {"name": "mason"},
}


class Humanoid:
    def __init__(self) -> None:
        pass


class Relationship:
    def __init__(self, id) -> None:
        self.length = 0
        self.id = id


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
            "None" if self.lover.id is None else self.lover.id,
            "\n",
            "loverName: ",
            "None" if self.lover.id is None else entities[self.lover.id].name,
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
            and self.lover.id is not None
        ):

            pregnancyChance = random.uniform(0.15, 0.25)

            if random.random() < pregnancyChance:

                self.isPregnant = True
                self.currentPregnancyLength = 0
        elif self.currentPregnancyLength >= self.maxPregnancyLength:
            self.isPregnant = False
            self.currentPregnancyLength = 0

            self.giveBirth([self.id, self.lover.id])
        elif self.isPregnant:
            self.currentPregnancyLength = self.currentPregnancyLength + 1

    def deathCheck(self):
        if not self.isDead and self.age[0] >= self.maxAge:
            if self.isPregnant and self.currentPregnancyLength >= 8:
                self.giveBirth([self.id, self.lover.id])
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

        if self.lover.id != "":

            self.lover.length = self.lover.length + 1
        if self.partner.id != "":
            self.partner.length = self.partner.length + 1
        if self.married.id != "":
            self.married.length = self.married.length + 1
        if self.lover.id == "" and self.age[0] >= self.breedAge:

            gender = self.gender

            if gender == "m" and availableLoversF:

                choice = random.choice(availableLoversF)

                self.lover.id = entities[choice].id
            elif gender == "f" and availableLoversM:

                choice = random.choice(availableLoversM)

                self.lover.id = entities[choice].id


def cycle():
    global availableLoversM
    global availableLoversF
    availableLoversM = []
    availableLoversF = []

    for entity in entities:
        lenM = len(availableLoversM)
        lenF = len(availableLoversF)
        if lenM >= 10 and lenF >= 10:
            pass
        elif entities[entity].isDead:
            pass
        elif (
            entities[entity].gender == "m"
            and entities[entity].lover.id == ""
            and lenM < 10
        ):
            availableLoversM.append(entities[entity].id)
        elif (
            entities[entity].gender == "f"
            and entities[entity].lover.id == ""
            and lenF < 10
        ):
            availableLoversF.append(entities[entity].id)

    entitiesCopy = entities.copy()
    for key in entitiesCopy:
        entitiesCopy[key].singleCycle()
    availableLoversF.clear()
    availableLoversM.clear()


def progress(count, total, status=""):
    bar_len = 60
    filled_len = int(round(bar_len * count / float(total)))

    percents = round(100.0 * count / float(total), 1)
    bar = "=" * filled_len + "-" * (bar_len - filled_len)

    sys.stdout.write("[%s] %s%s ...%s\r" % (bar, percents, "%", status))
    sys.stdout.flush()
    if count == total - 1:
        print("\n\n")


def save_object(obj, filename):
    with open(filename, "wb") as outp:  # Overwrites any existing file.
        pickle.dump(obj, outp, pickle.HIGHEST_PROTOCOL)


def generateSeeds(seedAmount):
    for seed in range(seedAmount):
        seedM = Human("m", "seed", True, True)
        seedF = Human("f", "seed", True, True)
        seedM.partner.id = seedF.id
        seedF.partner.id = seedM.id
        seedM.lover.id = seedF.id
        seedF.lover.id = seedM.id
        entities[seedM.id] = seedM
        entities[seedF.id] = seedF
        seedF.name[1] = entities[seedF.partner.id].name[1]


# def createFamilyTree():


def mainLoop():
    folder_path = "entities"

    for root, dirs, files in os.walk(folder_path, topdown=False):
        for name in files:
            if name.endswith(".pkl"):
                print("Removing: ", name)
                os.remove(os.path.join(root, name))

    start_time = time.time()
    seedAmount = 5
    months = 2444
    global entities
    entities = {}

    if months > 1200:
        chunkCyclesWhole = months / 1200
        chunkCyclesRemainder = months % 1200

        for month in range(math.floor(chunkCyclesWhole)):
            generateSeeds(seedAmount)
            progress(month + 1, chunkCyclesWhole, "cycle: " + str(month))
            for x in range(1200):
                progress(x, 1200, "cycle: " + str(x))
                cycle()
            save_object(entities, "entities/entities_" + str(month) + ".pkl")

            entities.clear()
        entities.clear()

        generateSeeds(seedAmount)
        for ex in range(chunkCyclesRemainder):
            progress(ex, chunkCyclesRemainder, "cycle: " + str(ex))
            cycle()
        save_object(
            entities,
            "entities/entities_" + str(math.floor(chunkCyclesWhole + 1)) + ".pkl",
        )
        entities.clear()
    else:
        for x in range(months):
            progress(x, months, "cycle: " + str(x))
            cycle()
            save_object(entities, "entities/entities_0.pkl")

            entities.clear()
    print("\n")
    print("--- %s seconds ---" % (time.time() - start_time))
    living = [(key) for (key) in entities if entities[key].isDead == False]
    print("seed amount: ", seedAmount)
    print("months: ", months)
    print("years: ", months / 12)
    livingTotal = []
    totalEntities = []
    totalSize = []
    path = "entities"
    for root, dirs, files in os.walk(path, topdown=False):
        for name in files:
            if name.endswith(".pkl"):
                with open(os.path.join(root, name), "rb") as inp:
                    f = pickle.load(inp)
                    total = len(f)
                    living = [(key) for (key) in f if f[key].isDead == False]
                    livingTotal.append(len(living))
                    totalEntities.append(total)
                    entitiesSizeOf = sys.getsizeof(f)
                    totalSize.append(entitiesSizeOf)
    print("living: ", sum(livingTotal))
    print("dead: ", sum(totalEntities) - sum(livingTotal))
    print("total: ", sum(totalEntities))
    print("size of entities(system: BYTES): ", sum(totalSize))
    print("size of entities(system: KILOBYTES): ", sum(totalSize) / 1000)
    print("size of entities(system: MEGABYTES): ", sum(totalSize) / 1000000)


mainLoop()
