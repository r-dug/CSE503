import pygame

city = input("enter a city: ")
date = input("enter a date: ")

with open("./weather_temps.csv", "r") as f:
    for line in f:
        line = f.readline().strip('\n').split(',')
        # print(line)
        if city in line and date in line:
            # print("found")
            avg_high = line[2]
            avg_low = line[3]
            print(f"The average high is {avg_high} and the average low is {avg_low}")

