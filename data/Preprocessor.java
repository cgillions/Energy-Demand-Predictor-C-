import java.io.File;
import java.io.IOException;
import java.util.Scanner;
import java.util.Calendar;
import java.text.SimpleDateFormat;
import java.text.ParseException;
import java.nio.file.StandardOpenOption;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.nio.file.Path;


public class Preprocessor {

	public static void main(String[] args) {

		// Get the input file.
		File inputFile = new File("2012-2017.csv");

		// Create output files.
		File trainFile 	    = createFile("train.csv"),
		     testFile  	    = createFile("test.csv"),
		     validationFile = createFile("validation.csv");

		// Keep a reference to the output paths.
		Path trainPath 	    = Paths.get(trainFile.getAbsolutePath()),
		     testPath  	    = Paths.get(testFile.getAbsolutePath()),
		     validationPath = Paths.get(validationFile.getAbsolutePath());

		try {
			// Read the data from the file.
			Scanner scanner = new Scanner(inputFile);

			// Create a formatter to parse a date string.
    	    SimpleDateFormat formatter = new SimpleDateFormat("yyyy-MM-dd");

    	    // Format the data into "dayOfYear,hourOfDay,dayOfWeek,demand".
    	    String   output;
			Calendar calendar;
            String[] timestampValue;

    	    int count = 0, currentHour = 0, avgDemand = 0, avgCount = 0,
			    year, dayOfYear, hourOfDay, dayOfWeek;

			while (scanner.hasNextLine()) {
				timestampValue = scanner.nextLine().split(",");

    		    // Strip whitespace and validate the length of the line.
    		    String dateTime = timestampValue[0].replaceAll("\\s", "");
		        if (dateTime.length() != 18) {
    		    	continue;
    		    }

    		    // Pull each element out of the string.
    		    String dateString = dateTime.substring(0,  10);
    		    String timestamp  = dateTime.substring(10, 18);

		        // Split each part into its subcomponents.
    		    String[] date = dateString.split("-");
    		    String[] time = timestamp.split(":");

    		    // Use Calendar library to get dayOfYear and dayOfWeek.
    		    calendar  = Calendar.getInstance();
    		    calendar.setTimeInMillis(formatter.parse(dateString).getTime());

		        year      = calendar.get(Calendar.YEAR);
    		    dayOfYear = calendar.get(Calendar.DAY_OF_YEAR);
    		    dayOfWeek = calendar.get(Calendar.DAY_OF_WEEK);

    		    // Get the hour of day.
		        hourOfDay = Integer.valueOf(time[0]);

		        if (hourOfDay == currentHour) {
		            // Contribute to the average demand for this hour.
    		    	avgDemand += Integer.valueOf(timestampValue[1]);
		        	avgCount++;
    		    } else {
                    // This is a reading for a new hour.
		        	// Save the previous hour.
                    avgDemand /= avgCount;
		        	output = String.format("%d,%d,%d,%d\n", dayOfYear, hourOfDay, dayOfWeek, avgDemand);

			        // Check if the data is for the validation dat set.

			    	if (year == 2017) {
    		    	    Files.write(
    	        		    validationPath,
    		    	    	output.getBytes(),
    	        			StandardOpenOption.APPEND
    	        		);
        	    	} else {
                	    // Output the data to a file. 66% of data is training, 33% testing.
        	    		Files.write(count++ % 3 == 0 ?
        	    	        testPath : trainPath,
        	        	    output.getBytes(),
        	    		   	StandardOpenOption.APPEND
        	        	);
        	    	}

	                // Reset the average counts.
	                currentHour = hourOfDay;
			        avgDemand   = Integer.valueOf(timestampValue[1]);
	    	        avgCount    = 1;
		        }
    	    }
		} catch (IOException ex) {
    	    System.out.println("IO Error! " + ex.getMessage());
    	    ex.printStackTrace();
		} catch (ParseException ex) {
    	    System.out.println("Parse Error! " + ex.getMessage());
    	    ex.printStackTrace();
		}
	}

	private static File createFile(String filename) {
  	    System.out.println("Creating file: " + filename);
	  	File file = new File(filename);
	  	try {
		    if (file.exists()) {
		    	file.delete();
	        }
	        file.createNewFile();

	    } catch (IOException ex) {
			System.out.println("Error! " + ex.getMessage());
			ex.printStackTrace();
	    }
        return file;
	}
}
