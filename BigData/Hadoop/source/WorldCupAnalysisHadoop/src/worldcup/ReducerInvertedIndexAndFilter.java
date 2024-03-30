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
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Reducer;

/*
*   Reducer class that only concat all the PlayerMatchGoal in input for a player.
*/

class ReducerInvertedIndexAndFilter extends Reducer <Text, PlayerMatchGoal, Text, Text> {
//    Instead of allocating a new variable each time within the function, we allocate them 
//    within the class, only changing their values within the relevant method.
    private Text invIndex = new Text();

    @Override
    protected void reduce(Text key, Iterable<PlayerMatchGoal> values, Context context) throws IOException, InterruptedException {
        invIndex.set("");

        for (PlayerMatchGoal value : values) {
            invIndex.set(invIndex.toString().concat("{" + value + "}; "));
        }
        
        context.write(key, invIndex);
    }
}
