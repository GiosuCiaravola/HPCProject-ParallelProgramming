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
import java.util.TreeMap;
import org.apache.hadoop.io.*;
import org.apache.hadoop.mapreduce.Reducer;

/*
*   Reducer class that receives elements from the mappers 
*   for their tops and builds a final "total" top in the same way.
*/

public class ReducerTopKScorers extends Reducer<NullWritable, PlayerTotalGoals,Text, Text> {
    private static Integer k;
    private Text playerName = new Text();
    private Text goals = new Text();
      
    @Override
    protected void setup(Context context) {
        k = Integer.valueOf(context.getConfiguration().get("k"));
    }

//    We can note that the initialization of the data structure and the final writing are within the function,
//    as there is only one group with a null key. Therefore, the reducer will be executed only once 
//    with all the elements in the values.
    @Override
    public void reduce(NullWritable key, Iterable<PlayerTotalGoals> values, Context context) throws IOException, InterruptedException {
        TreeMap<PlayerTotalGoals, String> topK = new TreeMap();

        for(PlayerTotalGoals record: values) {
            PlayerTotalGoals newValue = new PlayerTotalGoals(record.getPlayerName(), record.getGoalsNumber(), record.getGoals());
            topK.put(newValue, null);
            if(topK.size() > k) {
                topK.remove(topK.lastKey());
            }
        }

        for(PlayerTotalGoals mapKey: topK.keySet()){
            playerName.set(mapKey.getPlayerName());
            goals.set(mapKey.getGoals());
            context.write(playerName, goals);
        }
    }
}