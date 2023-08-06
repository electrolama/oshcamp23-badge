import math
import matplotlib.pyplot as plt


def generate_arc_points(increments, radius, x_offset, y_offset):
    points = []
    angle_increment = math.pi / 2 / increments
    for i in range(increments + 1):
        angle = i * angle_increment
        x = radius * math.cos(angle) + x_offset
        y = radius * math.sin(angle) + y_offset
        points.append((x, y))
    return points


# Example usage
num_increments = 6
radius = 38.25
x_offset = 0
y_offset = 0
arc_points = generate_arc_points(num_increments, radius, x_offset, y_offset)


# Plotting the points
x_values = [point[0] for point in arc_points]
y_values = [point[1] for point in arc_points]

plt.plot(x_values, y_values, "o-")
plt.xlabel("X")
plt.ylabel("Y")
plt.title("Points along a 90-degree arc")
plt.grid(True)

# Save the plot as a PNG file
plt.savefig("arc_points.png")

# Print the points
for point in arc_points:
    print("{}, {}".format(round(point[0], 2), round(point[1], 2)))
