
#;; A silly (but somewhat amusing) demo that changes the background colour
#;; over a range of colours.
#;;
#;; The scene is spun too, for extra eye-candy (so have a molecule
#;; loaded before you run this).
#;;
#;; What's the point?  It's to show that we can display the result of
#;; an arbitarily complex computation, i.e, we have a real extension
#;; language, not just a list of commands (like some other molecular
#;; graphics programs).

# n-steps is divided into 3 sets:
n_steps = 300.0

def background_demo():

    def rgb_colours(nlocal):

	r_step = 1 / n_steps
        red = 0.0
        green =0.0
        blue = 0.0

        if (n_local < (n_steps / 3)):
            red = 3 * r_step * n_local
            green = 0.0
        elif (n_local < ((2 * n_steps) / 3)):
            red = 1 - ((n_local - (n_steps / 3)) * 3 * r_step) 
            green = ((n_local - (n_steps / 3)) * 3 * r_step)
        else:
            red = 0.0
            green = 1 - (n_local - ((n_steps / 3) * 2)) * 3 * r_step

	return red,green,blue


    n_local = 1.0
    while (n_local<=n_steps):
	      r_col = rgb_colours(n_local)
# 	      print "colours ",r_col
	      set_background_colour(r_col[0],r_col[1],r_col[2])
	      rotate_y_scene(10,0.1) # n-frames frame-interval(degrees)
	      n_local = n_local + 1.0

background_demo()


