import matplotlib.pyplot as plt
import numpy as np

def update_plot(event):
    # Get mouse cursor position
    cursor_position_x = event.xdata
    cursor_position_y = event.ydata

    if cursor_position_x is not None and cursor_position_y is not None:
        # Calculate tan inverse
        angle = np.arctan2(cursor_position_y, cursor_position_x)

        # Update polar plot
        ax.clear()
        ax.set_theta_zero_location('N')
        ax.set_rlabel_position(90)
        ax.plot(angle, 1, 'ro')  # Plot a point at the calculated angle

        # Draw a line from the origin to the calculated point
        ax.plot([0, angle], [0, 1], color='blue', linestyle='dashed', linewidth=2)

        # Set axis limits
        ax.set_xlim([-np.pi, np.pi])
        ax.set_ylim([0, 1.5])

        # Show the plot
        plt.draw()

# Create a figure and axis
fig, ax = plt.subplots(subplot_kw={'projection': 'polar'})

# Connect the mouse motion event to the update_plot function
fig.canvas.mpl_connect('motion_notify_event', update_plot)

# Show the plot
plt.show()
