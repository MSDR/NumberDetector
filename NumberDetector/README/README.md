A personal introductory project to machine learning, this program is capable of identifying drawings using a convolutional neural network.

The network is currently built to recognize digits 0-9, hence the name NumberDetector.

Currently the .exe is unstable, so I recommend launching from an IDE. VS2017 is my preferred IDE.

To use:
  D / Left Mouse Button - Draw on the canvas.
  
  RSHIFT - Switches between training mode [default] and guessing mode.
  
  F - Passes current canvas through the network, producing a guess and table of confidences for each value. 
      If in training mode, the network will train from the canvas as well.
  E - Adds current canvas into memory, clears the current canvas.
  Z - Clears the current canvas.
  
  T - Trains the network from loaded data file.
  L - Appends canvases from data file to all canvases in memory.
  S - Prints all canvases in memory.
  W - Appends canvases in memory to data file.
      If LSHIFT is held, the file will be replaced with canvases in memory. This requires confirmation.
      
  DEL - Exit the program.

The network structure consists of 4 layers:
  Centering - Centers drawing on the canvas.
  Convolution - Performs convolution with 8 separate 3x3 filters.
  Pooling - Pools the 8 results from convolution.
  Softmax - Takes the pooling layer's result and produces 10 'confidences' between 0 and 1, the highest of which is the network's 'guess'.
  
  More detailed information is available in the accompanying images in the README folder. Apologies for the poor quality scans.
