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

import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.io.*;
import java.io.IOException;
import java.util.TreeMap;

/*
*   Mapper class that for each row (player) count the number of goal and build the ranking based on this number.
*   In tie cases we use the player name alphabetical order.
*/

public class MapperTopKScorers extends Mapper<Text, Text, NullWritable, PlayerTotalGoals> {
//    Constant for the ranking size.
    private static Integer k;
    private TreeMap<PlayerTotalGoals, String> topK;
      
//    Inizialize the top before all the executions.
    @Override
    protected void setup(Context context) {
//        Get the ranking size from the context.
        k = Integer.valueOf(context.getConfiguration().get("k"));
        topK = new TreeMap();
    }
      
  @Override
    public void map(Text key, Text value, Context context) throws IOException, InterruptedException {
//        We take the goals number from the number of element "G" and "P" thanks to the this split series.
        Integer goalsNumber = 0;
        for(String matchGoal: value.toString().split("; ")) 
            goalsNumber += matchGoal.split(":")[1].split(",").length;
        
//        Create a new PlayerTotalGoals object with the current row data.
        PlayerTotalGoals playerTotalGoals = new PlayerTotalGoals(key.toString(), goalsNumber, value.toString());
        topK.put(playerTotalGoals, null);

//        Remove the last key from the ranking if the size is bigger than the selected k.
        if(topK.size() > k)
          topK.remove(topK.lastKey());
    }
    
  @Override
    protected void cleanup(Context context) throws IOException, InterruptedException {
//        Write all the PlayerTotalGoals from the top in the context.
        for(PlayerTotalGoals key: topK.keySet())
            context.write(NullWritable.get(), key);
    }
  }
