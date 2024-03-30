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

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.*;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.lib.input.*;
import org.apache.hadoop.mapreduce.lib.output.*;

/*
*   Driver to manage the two Jobs that make up this program: the first one is used to obtain all the players who scored 
*   with pairings in all the matches they did; the second one to build the top.
*/

public class DriverTopScorersWithInvertedIndex {

    public static void main(String args[]) throws Exception {
        
//        Check if there are the right number of command line parameter.
        if (args.length < 4) {
            System.err.println("Usage: WorldCupAnalisysHadoop <inputFilePath> <firstJobOutputDir> <secondJobOutputDir> <NumberTopElement>");
            System.exit(-1);
        }

//        First command line parameter: dataset's path.
        Path inputDatasetPath = new Path(args[0]);
//        Second command line parameter: output directory path for the first job.
        Path outputDirInvInd = new Path(args[1]);
//        Third command line parameter: output directory path for the second job.
        Path outputDirTopK = new Path(args[2]);

        Configuration conf = new Configuration();

//        First job: InvertedIndex + Filter.
        Job job = Job.getInstance(conf);
        
        job.setJobName("World Cup Analisys - Step 1: Inverted Index");
        
//        Set path of the input dataset file for this job.
        FileInputFormat.addInputPath(job, inputDatasetPath);

//        Set path of the output folder for this job.
        FileOutputFormat.setOutputPath(job, outputDirInvInd);

//        Specify the class of the Driver for this job (this one).
        job.setJarByClass(DriverTopScorersWithInvertedIndex.class);

//        Set job input format: Text because in the dataset there are different column to split.
        job.setInputFormatClass(TextInputFormat.class);

//        Set job output format.
        job.setOutputFormatClass(TextOutputFormat.class);

//        Set map class.
        job.setMapperClass(MapperInvertedIndexAndFilter.class);

//        Set map output key and value classes: Text for the player name in the key; PlayerMatchGoal for the player match's goal.
        job.setMapOutputKeyClass(Text.class);
        job.setMapOutputValueClass(PlayerMatchGoal.class);

//        Set reduce class.
        job.setReducerClass(ReducerInvertedIndexAndFilter.class);

//        Set reduce output key and value classes: Text for the player name in the key; Text for the concat of all the player matches goals in the value.
        job.setOutputKeyClass(Text.class);
        job.setOutputValueClass(Text.class);

//        Execute the job and wait for completion.
        if (!job.waitForCompletion(true)) {
            System.exit(1);
         }

        Configuration conf2 = new Configuration();
        conf2.set("k", args[3]);
        
//        Second job: TopK.
        Job job2 = Job.getInstance(conf2);

        job2.setJobName("World Cup Project - Step 2: TopK");

//        Set path of the input folder for this job (output folder of the previous job).
        FileInputFormat.addInputPath(job2, outputDirInvInd);

//        Set path of the output folder for this job.
        FileOutputFormat.setOutputPath(job2, outputDirTopK);

//        Specify the class of the Driver for this job (this one).
        job2.setJarByClass(DriverTopScorersWithInvertedIndex.class);

//        Set job input format: KeyValue because, from the previous job, we know that the first fields is the 
//        player name and the second one is the concat of all the player matches goals.
        job2.setInputFormatClass(KeyValueTextInputFormat.class);

//        Set job output format
        job2.setOutputFormatClass(TextOutputFormat.class);

//        Set map class
        job2.setMapperClass(MapperTopKScorers.class);

//        Set map output key and value classes: NullWritable for the key as the TopK pattern includes; 
//        PlayerTotalGoals fo the value because is the object of the Top.
        job2.setMapOutputKeyClass(NullWritable.class);
        job2.setMapOutputValueClass(PlayerTotalGoals.class);

//        Set reduce class
        job2.setReducerClass(ReducerTopKScorers.class);

//        Set reduce output key and value classes: Text for the player name in the key; Text for all the player matches goals in the value.
        job2.setOutputKeyClass(Text.class);
        job2.setOutputValueClass(Text.class);

//        Execute the job and wait for completion
        System.exit(job2.waitForCompletion(true) ? 0 : 1);
    }
}
