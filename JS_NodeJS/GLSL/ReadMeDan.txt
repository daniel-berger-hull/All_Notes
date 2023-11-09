
Pure WebGL Examples
===================


Jan 13, 2023:
-------------
  This is site has extraordinary randering done in WebGL!

    https://webglfundamentals.org/webgl/lessons/webgl-less-code-more-fun.html



The fodler examples at the following locations cover the basics of WebGL, and could be used a fundation for my WebGL/GLSL projects.

1- Learn-WebGL

  Location: --> D:\data\workspace\JavaScriptGraphics\WebGL\Learn-WebGL

  Comment: Simple a straight implementation, very simple html file, and has a bit more examples than the other on this section

  TO DO: 
     --> GLSL code is within the Javascript file, and is even push directly as string parameter to the gl.shaderSource call (which is too much to be redable)     
         Should be extracted into their own vertex and fragment shader files (.glsl)...

  Videos are here:  https://www.youtube.com/playlist?list=PL2935W76vRNHFpPUuqmLoGCzwx_8eq5yK
  

2 -   IndigoCS-webgl-tutorials

    Location: --> D:\data\workspace\JavaScriptGraphics\WebGL\IndigoCS-webgl-tutorials
    
    Comment: As the previous (1), simple code, and further more, the GLSL code is seperated into its own array variable instead of be pushed a param to the gl.shaderSource
    
    TO DO:
     --> Also the shader code should be into its own .glsl file
    
 
  
Dec 27 2022 -->  WINNER is D:\data\workspace\JavaScriptGraphics\WebGL\Learn-WebGL!

 It has videos to explain, code if fairly straight

WebGL+GLSL: Great and complete Web site that explains everthing:
  
    http://learnwebgl.brown37.net/index.html

=============================================================================================

Notes:

multiple object to render:
  --> call multiple time the drawArrays or gl.drawElements!
  
  Check the example of 
     D:\data\workspace\JavaScriptGraphics\WebGL\uDemy\Code_Examples\public\