#--------------

import turtle

class TurtlePractice:
    def __init__(self):
        self.window = turtle.Screen()
        self.window.bgcolor("cornsilk")

        self.tt = turtle.Turtle()
        turtle.mode("logo")
        self.tt.shape("classic")
        self.tt.pensize(5)
        self.tt.speed(5)
        self.tt.penup()
        self.tt.goto(0, -200)

        self.rachsis_length = 10.0
        self.total_rachsis_length = 100
        self.l_barb_length = 10
        self.r_barb_length = 15
        self.barb_seg_length = 1
        self.outline = 10.0

        self.currentLength=0




    def run(self):
        landMark = (0,-200)
        N = self.total_rachsis_length
        Ml = self.l_barb_length
        Mr = self.r_barb_length

        drawn_rachsis_length = 0


        #R(0)
        self.tt.setheading(0)
        drawSegment(self.rachsis_length, self.tt)
        drawn_rachsis_length += self.rachsis_length  # increase drawn length
        while drawn_rachsis_length < N:
            print(drawn_rachsis_length)
            self.tt.color("black")
            landMark = self.tt.pos()
            #Bl
            drawn_barbL_length = 0
            drawn_barbR_length = 0
            while drawn_barbL_length < Ml:
                self.tt.setheading(-90)
                drawSegment(self.barb_seg_length, self.tt)
                drawn_barbL_length += self.barb_seg_length
            self.tt.goto(landMark)
            #Br
            while drawn_barbR_length < Mr:
                self.tt.setheading(90)
                drawSegment(self.barb_seg_length, self.tt)
                drawn_barbR_length += self.barb_seg_length
            self.tt.goto(landMark)
            #R(i+1)
            self.tt.color("red")
            self.tt.setheading(0)
            drawSegment(self.rachsis_length, self.tt)
            drawn_rachsis_length += self.rachsis_length  # increase drawn length
            print(drawn_rachsis_length)









def drawSegment(length, turtle):
    turtle.pendown()
    turtle.forward(length)
    turtle.penup()



if __name__ == "__main__":
    s = TurtlePractice()
    s.run()
    s.window.exitonclick()
