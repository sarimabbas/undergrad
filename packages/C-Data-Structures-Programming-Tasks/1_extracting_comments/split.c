/*
 * Reads audio data as decimal integers from standard input and outputs
 * the intervals corresponding to tracks.  Tracks are maximal intervals
 * of samples that contain no long runs of low values.  The starting
 * and ending point of each track is written to standard output.
 */

#include <stdio.h>
#include <stdlib.h>

void output_interval(int start, int end, int rate);

/* states for the FSM:
 * GAP = in the gap between tracks
 * TRACK = in a track
 * ZEROS = in a run of zeros (or low values at or below the threshold)
 */
typedef enum {GAP, TRACK, ZEROS} state;

int main(int argc, char **argv)
{
  // the current state of the FSM
  state curr = GAP;

  // the index of the currently read sample
  int count = 0;

  // the maximum value to count as silence
  const int threshold = 5;

  // the sample rate of the audio being read
  const int sample_rate = 44100;

  // the minimum length of a gap between tracks, in seconds and samples
  const double min_gap_sec = 0.0001;
  const int min_gap_samples = (int)(sample_rate * min_gap_sec);

  // the starting and ending points of the most recent track read or being read
  int start, end;

  // the current sample value
  int value;
  while (scanf("%d", &value) > 0)
    {
      switch (curr)
	{
	case GAP:
	  if (abs(value) > threshold)
	    {
	      // end of gap; now reading track so remember start
	      start = count;
	      curr = TRACK;
	    }
	  break;

	case TRACK:
	  if (abs(value) <= threshold)
	    {
	      // read a low value, start counting consecutive low values
	      // and record possible end of current track
	      end = count - 1;
	      curr = ZEROS;
	    }
	  break;

	case ZEROS:
	  if (abs(value) > threshold)
	    {
	      // read a high value; just a pause in the track so read some more
	      curr = TRACK;
	    }
	  else if (count - end >= min_gap_samples)
	    {
	      // read enough consecutive low values to consider track over;
	      // output interval for last track and continue to read through gap
	      curr = GAP;
	      output_interval(start, end, sample_rate);
	    }
	}

      count++;
    }

  // output interval for last track if input ended in the middle of a track
  if (curr == TRACK)
    {
      end = count - 1;
    }

  if (curr == TRACK || curr == ZEROS)
    {
      output_interval(start, end, sample_rate);
    }
}

/**
 * Writes the given interval to standard output.  The interval is given by sample index and
 * the output in converted to seconds by dividing by the given rate.
 *
 * @param start an integer
 * @param end an integer
 * @param rate a non-zero integer
 */
void output_interval(int start, int end, int rate)
{
  printf("[%lf-%lf]\n", (double)start / rate, (double)end / rate);
}
