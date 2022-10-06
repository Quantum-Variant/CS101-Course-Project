#include <simplecpp>
#include <sstream>
#include "shooter.h"
#include "bubble.h"

/* Simulation Vars */
const double STEP_TIME = 0.02;

/* Game Vars */
const int PLAY_Y_HEIGHT = 450;
const int LEFT_MARGIN = 70;
const int TOP_MARGIN = 50;
const int BOTTOM_MARGIN = (PLAY_Y_HEIGHT+20);

int level_complete=0, shooter_hit=0, score_total=0, score_factor=500;
char lives='3';


bool dist_bubble_bullet(int bulx, int buly, int bubx, int buby){
    //To find distance between bubbles and bullets
    double dist = (int)sqrt((bulx-bubx)*(bulx-bubx) + (buly-buby)*(buly-buby));
    if(dist<=15.0)
            return true; //If a bullet has hit a bubble
    return false;
}

bool dist_bubble_shooter(vector<Bubble> &bubbles, Shooter shooter){
    //To find distance between bubbles and shooter
    for(unsigned int i=0; i<bubbles.size(); i++)
    {
        int bx=bubbles[i].get_center_x(), by=bubbles[i].get_center_y(), shx=shooter.get_head_center_x(), shy=shooter.get_head_center_y(), sbx=shooter.get_body_center_x(), sby=shooter.get_body_center_y();
        double dist_head = (int)sqrt((bx-shx)*(bx-shx) + (by-shy)*(by-shy));
        double dist_body = (int)sqrt((bx-sbx)*(bx-sbx) + (by-sby)*(by-sby));
        if(dist_head<=20.0 || dist_body<=20.0)
            return true; //If a bubble has hit the shooter
    }

    return false;
}

void move_bullets(vector<Bullet> &bullets, vector<Bubble> &bubbles){
    // move all bullets
    for(unsigned int i=0; i<bullets.size(); i++){
        if(!bullets[i].nextStep(STEP_TIME))
            bullets.erase(bullets.begin()+i);
        for(unsigned int j=0; j<bubbles.size(); j++)
        {
            if(dist_bubble_bullet(bullets[i].get_center_x(), bullets[i].get_center_y(), bubbles[j].get_center_x(), bubbles[j].get_center_y()) == true)
            { //If a bullet has hit a bubble
                bullets.erase(bullets.begin()+i);
                bubbles.erase(bubbles.begin()+j);

                score_total+=score_factor; //Increasing score

                if(bubbles.size()==0)
                {
                    level_complete=1;
                }

                break;
            }
        }
    }
}

void move_bubbles(vector<Bubble> &bubbles){
    // move all bubbles
    for (unsigned int i=0; i < bubbles.size(); i++)
    {
        bubbles[i].nextStep(STEP_TIME);
    }
}

void check_bubbles_hit_shooter(vector<Bubble> &bubbles, Shooter shooter){
    //to check if any bubble has hit the shooter
    if(dist_bubble_shooter(bubbles, shooter) == true)
        shooter_hit = 1;
}

vector<Bubble> create_bubbles(int level_num)
{
    // create initial bubbles in the game
    vector<Bubble> bubbles;
    if(level_num == 1)
    {
        bubbles.push_back(Bubble(WINDOW_X/2.0, BUBBLE_START_Y, BUBBLE_DEFAULT_RADIUS, -BUBBLE_DEFAULT_VX, -BUBBLE_DEFAULT_VY, COLOR(255,105,180)));
        bubbles.push_back(Bubble(WINDOW_X/4.0, BUBBLE_START_Y, BUBBLE_DEFAULT_RADIUS, BUBBLE_DEFAULT_VX, BUBBLE_DEFAULT_VY, COLOR(255,105,180))); //adding bubbles into vector
    }
    else if(level_num == 2)
    {
        bubbles.push_back(Bubble(WINDOW_X/2.0, BUBBLE_START_Y, BUBBLE_DEFAULT_RADIUS, -BUBBLE_DEFAULT_VX, -BUBBLE_DEFAULT_VY, COLOR(255,200,50)));
        bubbles.push_back(Bubble(WINDOW_X/4.0, BUBBLE_START_Y, BUBBLE_DEFAULT_RADIUS, BUBBLE_DEFAULT_VX, BUBBLE_DEFAULT_VY, COLOR(255,200,50)));
        bubbles.push_back(Bubble(WINDOW_X/8.0, BUBBLE_START_Y, BUBBLE_DEFAULT_RADIUS, BUBBLE_DEFAULT_VX, BUBBLE_DEFAULT_VY, COLOR(255,200,50))); //adding bubbles into vector
    }
    else if(level_num == 3)
    {
        bubbles.push_back(Bubble(WINDOW_X/2.0, BUBBLE_START_Y, BUBBLE_DEFAULT_RADIUS, -BUBBLE_DEFAULT_VX, -BUBBLE_DEFAULT_VY, COLOR(0,200,255)));
        bubbles.push_back(Bubble(WINDOW_X/4.0, BUBBLE_START_Y, BUBBLE_DEFAULT_RADIUS, BUBBLE_DEFAULT_VX, BUBBLE_DEFAULT_VY, COLOR(0,255,200)));
        bubbles.push_back(Bubble(WINDOW_X/8.0, BUBBLE_START_Y, BUBBLE_DEFAULT_RADIUS, BUBBLE_DEFAULT_VX, BUBBLE_DEFAULT_VY, COLOR(0,200,255)));
        bubbles.push_back(Bubble(WINDOW_X*3.0/4.0, BUBBLE_START_Y, BUBBLE_DEFAULT_RADIUS, BUBBLE_DEFAULT_VX, BUBBLE_DEFAULT_VY, COLOR(0,255,200)));
        bubbles.push_back(Bubble(WINDOW_X*7.0/8.0, BUBBLE_START_Y, BUBBLE_DEFAULT_RADIUS, BUBBLE_DEFAULT_VX, BUBBLE_DEFAULT_VY, COLOR(0,200,255))); //adding bubbles into vector

    }
    return bubbles;
}


int main()
{
    initCanvas("Bubble Trouble", WINDOW_X, WINDOW_Y);

    Line b1(0, TOP_MARGIN, WINDOW_X, TOP_MARGIN);
    b1.setColor(COLOR(0, 0, 100));

    Line b2(0, PLAY_Y_HEIGHT, WINDOW_X, PLAY_Y_HEIGHT);
    b2.setColor(COLOR(0, 0, 100));

    XEvent event;

    // Main game loop
    int round=1;
    while(round<=3)
    {
        // Intialize the shooter
        Shooter shooter(SHOOTER_START_X, SHOOTER_START_Y, SHOOTER_VX);

        string msg_cmd("Cmd: _"); //string array with each character occupying one index
        Text charPressed(LEFT_MARGIN, BOTTOM_MARGIN, msg_cmd);

        string no_of_lives("Lives left: # # #"); //string array with count of number of lives left
        Text lives_left(400, TOP_MARGIN-15, no_of_lives);

        string score("Score: "); //string array with score of the player (cumulative for all levels)
        Text score_player(400, BOTTOM_MARGIN, score);
        //Using stringstream class to convert the integer representation of 'score' to string form
        string str_score;
        stringstream ss;
        ss << score_total;
        ss >> str_score;
        score_player.setMessage(score+str_score);

        char num = '\0';
        string timer("Time (sec): "); //string array with timer

        if(round == 1)
        {
            timer += "50/50"; //setting total time for the round
            num = '5';
        }
        else if(round == 2)
        {
            timer += "40/40"; //setting total time for the round
            num = '4';
        }
        else if(round == 3)
        {
            timer += "30/30"; //setting total time for the round
            num = '3';
        }

        Text time_left(LEFT_MARGIN, TOP_MARGIN-15, timer);
        char t[3]={num, '0'};
        int ctr=0; //to manage timer count


        string lno("Level _"); //string array with level number
        Text levelno(250, 250, lno);
        lno[lno.length()-1] = round+48;
        levelno.setMessage(lno);
        wait(3);
        levelno.hide(); //To remove message of level number

        // Initialize the bubbles
        vector<Bubble> bubbles = create_bubbles(round);

        // Initialize the bullets (keeping empty until bullet shot)
        vector<Bullet> bullets;

        for(unsigned int i=0; i<bubbles.size(); i++)
            bubbles[i].set_acceleration(round);

        while (true)
        {
            bool pendingEvent = checkEvent(event);
            if (pendingEvent)
            {
                // Get the key pressed
                char c = charFromEvent(event);
                msg_cmd[msg_cmd.length() - 1] = c;
                charPressed.setMessage(msg_cmd);

                // Update the shooter
                if(c == 'a')
                    shooter.move(STEP_TIME, true);
                else if(c == 'd')
                    shooter.move(STEP_TIME, false);
                else if(c == 'w')
                    bullets.push_back(shooter.shoot());
                else if(c == 'q')
                    return 0;
            }

            // Update the bubbles
            move_bubbles(bubbles);

            // Update the bullets
            move_bullets(bullets, bubbles);

            // Check if the shooter has been hit
            check_bubbles_hit_shooter(bubbles, shooter);

            //Updating number of lives left
            if(lives == '2')
                no_of_lives[no_of_lives.length()-1]=' ';
            else if(lives == '1')
                no_of_lives[no_of_lives.length()-3]=' ';

            lives_left.setMessage(no_of_lives);


            wait(STEP_TIME);

            timer[timer.length() - 5] = t[0];
            timer[timer.length() - 4] = t[1];
            time_left.setMessage(timer);

            stringstream ss;
            ss << score_total;
            ss >> str_score;
            score_player.setMessage(score+str_score);

            if(ctr%25 ==0)
            {
                ctr=0;
                t[1]--;
                if(t[1]==47)
                {
                    t[0]--;
                    t[1]='9';
                }
            }

            if(t[0] + round == 52)
                score_factor = 400;
            else if (t[0] + round == 51)
                score_factor = 300;
            else if (t[0] + round == 50)
                score_factor = 200;
            else if(t[0] + round == 49)
                score_factor = 100;

            if(t[0] == '0' && t[1]=='0') //if time up
            {
                for(unsigned int i=0; i<bullets.size(); i++)
                    bullets.erase(bullets.begin()+i);
                timer[timer.length() - 5] = t[0];
                timer[timer.length() - 4] = t[1];
                time_left.setMessage(timer);
                Text msg(250,250,"Time's up! Game Over");
                msg.setColor(COLOR("blue"));
                wait(5);
                break;
            }

            if(shooter_hit == 1) //If the shooter has been hit by a bubble
            {
                shooter.set_shooter_colour(COLOR("red"));
                while(dist_bubble_shooter(bubbles, shooter))
                {
                    move_bubbles(bubbles);
                    wait(STEP_TIME);
                }
                shooter.set_shooter_colour(COLOR(50, 255, 100));
                lives--;
            }
            shooter_hit = 0;

            if(lives == '0') //If the shooter has lost all his lives
            {
                for(unsigned int i=0; i<bullets.size(); i++)
                    bullets.erase(bullets.begin()+i);

                no_of_lives[no_of_lives.length()-5]=' '; //reducing number of lives to none
                lives_left.setMessage(no_of_lives);

                Text msg(250,250,"Game over!");
                msg.setColor(COLOR("red"));
                wait(5);
                break;
            }

            if(level_complete == 1) //if level completed
            {
                for(unsigned int i=0; i<bullets.size(); i++)
                    bullets.erase(bullets.begin()+i);
                Text msg(250,250,"Level Complete!");
                msg.setColor(COLOR(0, 155, 0));
                wait(5);
                msg.hide();

                if(round == 3)
                {
                    Text msg(250,250,"YOU WIN!");
                    msg.setColor(COLOR(200, 50, 200));
                    wait(5);
                }
                break;
            }

            ctr++;
        }

        //End game if lives lost or time up
        if(lives == '0' || (t[0]=='0' && t[1]=='0'))
            break;

        //Reset values
        level_complete=0;
        score_factor = 500;
        round++;
        ctr=0;
        lives = '3';
        t[0]='5';
        t[1]='0';
    }
}
