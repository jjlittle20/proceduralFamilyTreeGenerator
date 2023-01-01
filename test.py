from names import first_male_names, first_female_names, british_surnames
import time
import itertools
import random
import sys

base = [
    "id",  # 0 int
    "isSeed",  # 1 bool
    "parents",  # 2 [mother(id),father(id)]
    "gender",  # 3 "m" or "f"
    "age",  # 4 ["years","months"]
    "name",  # 5 ["first","last"]
    "isPregnant",  # 6 bool
    "isMarried",  # 7 bool
    "partner",  # 8 [int(id),int(length)]
    "marriedTo",  # 9 [int(id),int(length)]
    "lover",  # 10 [int(id),int(length)]
    "isDead",  # 11 bool
    "maidenName",  # 12 ["first","last"]
]
baseConstants = {
    "maxAge": 45,
    "breedAge": 18,
    "maxPregnancyLength": 12,
}


def getAge(isSeed=False):
    if isSeed:
        return [
            random.randint(baseConstants["breedAge"], baseConstants["breedAge"] + 10),
            0,
        ]
    else:
        return [0, 0]


def generateName(gender, isSeed, parents):

    name = ["", ""]

    if gender == "m":
        name[0] = random.choice(first_male_names)
    else:
        name[0] = random.choice(first_female_names)

    if isSeed:
        name[1] = random.choice(british_surnames).upper()

    else:

        name[1] = entities[parents[1]]["name"][1]

    return name


def pregnancyCheck(entity):

    if entity["gender"] == "m":
        return

    elif (
        entity["age"][0] >= baseConstants["breedAge"]
        and not entity["isPregnant"]
        and entity["lover"][0] != -1
    ):

        pregnancyChance = random.uniform(0.15, 0.25)

        if random.random() < pregnancyChance:

            entity["isPregnant"] = True
            entity["currentPregnancyLength"] = 0
    elif entity["currentPregnancyLength"] >= baseConstants["maxPregnancyLength"]:
        entity["isPregnant"] = False
        entity["currentPregnancyLength"] = 0

        giveBirth(entity, [entity["id"], entity["lover"][0]])
    elif entity["isPregnant"]:
        entity["currentPregnancyLength"] = entity["currentPregnancyLength"] + 1


def giveBirth(entity, parents):

    newHuman = {
        "id": next(id_iter),
        "isSeed": False,
        "parents": [parents[0], parents[1]],
        "gender": random.choice(["m", "f"]),
        "age": getAge(False),
        "name": "",
        "partner": [-1, 0],
        "marriedTo": [-1, 0],
        "lover": [-1, 0],
        "isDead": False,
        "maidenName": ["", ""],
        "isPregnant": False,
        "currentPregnancyLength": 0,
    }
    newHuman["name"] = generateName(newHuman["gender"], False, [parents[0], parents[1]])
    entities[newHuman["id"]] = newHuman


def generateSeeds(seedAmount):
    for seed in range(seedAmount):
        seedM = {
            "id": next(id_iter),
            "isSeed": True,
            "parents": [-1, -1],
            "gender": "m",
            "age": getAge(True),
            "name": generateName("m", True, [-1, -1]),
            "partner": [-1, 0],
            "marriedTo": [-1, 0],
            "lover": [-1, 0],
            "isDead": False,
            "maidenName": ["", ""],
            "isPregnant": False,
            "currentPregnancyLength": 0,
        }
        seedF = {
            "id": next(id_iter),
            "isSeed": True,
            "parents": [-1, -1],
            "gender": "f",
            "age": getAge(True),
            "name": generateName("m", True, [-1, -1]),
            "partner": [-1, 0],
            "marriedTo": [-1, 0],
            "lover": [-1, 0],
            "isDead": False,
            "maidenName": ["", ""],
            "isPregnant": False,
            "currentPregnancyLength": 0,
        }
        seedM["partner"][0] = seedF["id"]  # partner
        seedF["partner"][0] = seedM["id"]  # partner
        seedM["lover"][0] = seedF["id"]  # lover
        seedF["lover"][0] = seedM["id"]  # lover
        entities[seedM["id"]] = seedM
        entities[seedF["id"]] = seedF
        seedF["name"][1] = entities[seedF["partner"][0]]["name"][1]  # female surname


def deathCheck(entity):
    if not entity["isDead"] and entity["age"][0] >= baseConstants["maxAge"]:
        if entity["isPregnant"] and entity["currentPregnancyLength"] >= 8:
            giveBirth(entity, [entity["id"], entity["lover"][0]])
        entity["isDead"] = True
    else:
        if entity["age"][1] >= 12:
            entity["age"][0] = entity["age"][0] + 1
            entity["age"][1] = 0
        else:
            entity["age"][1] = entity["age"][1] + 1


def getAvalibleLovers():
    availableLoversM = []
    availableLoversF = []

    for entity in entities:
        lenM = len(availableLoversM)
        lenF = len(availableLoversF)
        if lenM >= 10 and lenF >= 10:
            pass
        elif entities[entity]["isDead"]:
            pass
        elif (
            entities[entity]["gender"] == "m"
            and entities[entity]["lover"][0] == -1
            and lenM < 10
        ):
            availableLoversM.append(entities[entity]["id"])
        elif (
            entities[entity]["gender"] == "f"
            and entities[entity]["lover"][0] == -1
            and lenF < 10
        ):
            availableLoversF.append(entities[entity]["id"])
    lovers = {"maleList": availableLoversM, "femaleList": availableLoversF}
    return lovers


def relationshipCheck(entity):

    if entity["lover"][0] != -1:

        entity["lover"][1] = entity["lover"][1] + 1
    if entity["partner"][0] != -1:
        entity["partner"][1] = entity["partner"][1] + 1
    if entity["marriedTo"][0] != -1:
        entity["marriedTo"][1] = entity["marriedTo"][1] + 1
    if entity["lover"][0] == -1 and entity["age"][0] >= baseConstants["breedAge"]:

        gender = entity["gender"]

        if gender == "m" and lovers["femaleList"]:

            choice = random.choice(lovers["femaleList"])

            entity["lover"][0] = choice
        elif gender == "f" and lovers["maleList"]:

            choice = random.choice(lovers["maleList"])

            entity["lover"][0] = choice


def progress(count, total, status=""):
    bar_len = 60
    filled_len = int(round(bar_len * count / float(total)))

    percents = round(100.0 * count / float(total), 1)
    bar = "=" * filled_len + "-" * (bar_len - filled_len)

    sys.stdout.write("[%s] %s%s ...%s\r" % (bar, percents, "%", status))
    sys.stdout.flush()
    if count == total - 1:
        print("\n\n")


def main():
    seedAmount = 5
    global id_iter
    id_iter = itertools.count()
    #
    global entities
    global lovers
    entities = {}
    start_time = time.time()
    generateSeeds(seedAmount)
    months = 2400
    for c in range(months):
        lovers = getAvalibleLovers()
        progress(c, months, "cycle: " + str(c))
        entcop = entities.copy()
        for e in entcop:
            relationshipCheck(entities[e])
            pregnancyCheck(entities[e])
            deathCheck(entities[e])

    print("total entites: ", len(entities))
    # for x in range(20000):

    #     print(next(id_iter))
    #     entities[x] = base
    # for x in range(20000):
    #     entities[x][1] = False
    print("\n")
    print("--- %s seconds ---" % (time.time() - start_time))


main()
