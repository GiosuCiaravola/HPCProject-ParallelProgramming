/*  
*   Ciaravola Giosuè, 0622702177, g.ciaravola3@studenti.unisa.it
*   Attended course: AH
*   Prof. D'Aniello Giuseppe, gidaniello@unisa.it
*   
*   Exercise 1 - Map Reduce
*
*   Develop a program using Hadoop Map Reduce to analyze the available dataset differently from the second exercise
*   (e.g., a statistic, a ranking, an index, etc.). 
*
*   For this task, the decision has been made to calculate the ranking of players who have scored the most goals overall. 
*   The number of players considered for the ranking is taken from the command line. 
*   Additionally, for each player, all the goals scored are listed, as an inverted index, indicating in which match they were made.
*/

package worldcup;

import java.io.IOException;
import org.apache.hadoop.io.*;
import org.apache.hadoop.mapreduce.Mapper;

/*
*   Mapper class that for each row, if the player has scored a goal, write in the context the player name,
*   paired to the match and goals of that row. We use LongWritable for the input key, just to ignore the
*   dataset header (offset = 0).
*/

public class MapperInvertedIndexAndFilter extends Mapper<LongWritable, Text, Text, PlayerMatchGoal> {
//    Instead of allocating a new variable each time within the function, we allocate them 
//    within the class, only changing their values within the relevant method.
    private PlayerMatchGoal matchGoals;
    private Text playerName = new Text();
    
    @Override
    protected void map(LongWritable key, Text value, Mapper.Context context) throws IOException, InterruptedException {
        
//        Check to ignore the header.
        if(key.get() == 0)
            return;
        
//        Split all the dataser columns.
        String[] fields = value.toString().split(",");
        
//        Flag used to check if the player has scored at least one goal, in order to initialize the PlayerMatchGoal 
//        object with the matchID and write it in the context.
        Boolean flag = true;
        
//        Check to ignore the raw without event (last column).
        if(fields.length == 9) {
//            Split all the events.
            for(String event: fields[8].split(" "))
//                Check if the event is a goal (or penalty).
                if(event.startsWith("G") || event.startsWith("P")) {
//                    Whe we find the first goal, initialize the object and change the flag value.
                    if(flag) {
                        matchGoals = new PlayerMatchGoal(fields[1]);
                        flag = false;
                    }
//                    Add the goal.
                    matchGoals.addGoal(event);
                }
        }

//        If the player scored at least one goal (filter) we write the pair (player name, match goals) in the context.
        if(!flag) {
            playerName.set(fields[6]);
            context.write(playerName, matchGoals);
        }
    }
}
