xof 0303txt 0032

Frame Root {
  FrameTransformMatrix {
     1.000000, 0.000000, 0.000000, 0.000000,
     0.000000,-0.000000, 1.000000, 0.000000,
     0.000000, 1.000000, 0.000000, 0.000000,
     0.000000, 0.000000, 0.000000, 1.000000;;
  }
  Frame cube {
    FrameTransformMatrix {
       1.000000, 0.000000, 0.000000, 0.000000,
       0.000000, 1.503770, 0.000000, 0.000000,
       0.000000, 0.000000, 0.058326, 0.000000,
       0.000000, 0.000000, 0.083224, 1.000000;;
    }
    Mesh { // cube mesh
      8;
      -1.000000;-1.000000;-1.000000;,
      -1.000000;-1.000000; 1.000000;,
      -1.000000; 1.000000;-1.000000;,
      -1.000000; 1.000000; 1.000000;,
       1.000000;-1.000000;-1.000000;,
       1.000000;-1.000000; 1.000000;,
       1.000000; 1.000000;-1.000000;,
       1.000000; 1.000000; 1.000000;;
      6;
      4;2,3,1,0;,
      4;6,7,3,2;,
      4;4,5,7,6;,
      4;0,1,5,4;,
      4;0,4,6,2;,
      4;5,1,3,7;;
      MeshNormals { // cube normals
        6;
        -1.000000;-0.000000; 0.000000;,
         0.000000; 1.000000; 0.000000;,
         1.000000;-0.000000; 0.000000;,
         0.000000;-1.000000; 0.000000;,
         0.000000; 0.000000;-1.000000;,
         0.000000;-0.000000; 1.000000;;
        6;
        4;0,0,0,0;,
        4;1,1,1,1;,
        4;2,2,2,2;,
        4;3,3,3,3;,
        4;4,4,4,4;,
        4;5,5,5,5;;
      } // End of cube normals
      MeshMaterialList { // cube material list
        1;
        6;
        0,
        0,
        0,
        0,
        0,
        0;
        Material mat {
           0.097788; 0.201884; 0.640000; 1.000000;;
           96.078431;
           0.500000; 0.500000; 0.500000;;
           0.000000; 0.000000; 0.000000;;
        }
      } // End of cube material list
    } // End of cube mesh
  } // End of cube
} // End of Root
