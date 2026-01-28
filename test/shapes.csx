<?xml version="1.0" encoding="UTF-8" standalone="yes" ?>
<openEMS>
  <FDTD NumberOfTimesteps="300000" endCriteria="0.0001" f_max="5000000000">
    <Excitation Type="0" f0="2510000000" fc="2490000000">
    </Excitation>
    <BoundaryCond xmin="PML_8" xmax="PML_8" ymin="PML_8" ymax="PML_8" zmin="PML_8" zmax="PML_8">
    </BoundaryCond>
  </FDTD>
  <ContinuousStructure CoordSystem="0">
    <Properties>
      <Metal Name="Shape">
        <Primitives>
          <MultiBox Priority="1">
            <StartP X="-60" Y="-60" Z="-2"/>
            <EndP X="-55" Y="-55" Z="-7"/>
            <StartP X="-50" Y="-50" Z="-2"/>
            <EndP X="-45" Y="-45" Z="-7"/>
            <StartP X="-40" Y="-40" Z="-2"/>
            <EndP X="-35" Y="-35" Z="-7"/>
          </MultiBox>
          <Box Priority="1">
            <P1 X="-25" Y="-25" Z="1"/>
            <P2 X="-30" Y="-30" Z="-1"/>
            <Transformation>
              <Rotate_Z Argument="0.7854"/>
              <Translate Argument="-25, 10, 0"/>
            </Transformation>
          </Box>
          <Box Priority="1">
            <P1 X="0" Y="0" Z="1"/>
            <P2 X="-20" Y="-20" Z="-1"/>
            <Transformation/>
          </Box>
          <Point Priority="1" X="1" Y="1" Z="1"/>
          <LinPoly Priority="1" Elevation="1" Length="3" NormDir="2">
            <Vertex X1="10" X2="10"/>
            <Vertex X1="10" X2="20"/>
            <Vertex X1="30" X2="20"/>
          </LinPoly>
          <Polygon Priority="1" Elevation="1" NormDir="2">
            <Vertex X1="35" X2="25"/>
            <Vertex X1="40" X2="25"/>
            <Vertex X1="40" X2="40"/>
          </Polygon>
          <Sphere Priority="1" Radius="5">
            <Center X="50" Y="50" Z="10"/>
          </Sphere>
          <Cylinder Priority="1" Radius="6">
            <P1 X="65" Y="65" Z="-10"/>
            <P2 X="65" Y="65" Z="-20"/>
          </Cylinder>
          <Cylinder Priority="1" Radius="6">
            <P1 X="80" Y="70" Z="30"/>
            <P2 X="80" Y="80" Z="40"/>
          </Cylinder>
          <Curve Priority="1">
            <Vertex X="90" Y="90" Z="50"/>
            <Vertex X="95" Y="95" Z="50"/>
            <Vertex X="100" Y="90" Z="50"/>
            <Vertex X="105" Y="95" Z="55"/>
          </Curve>
          <Wire Priority="1" WireRadius="0.2">
            <Vertex X="115" Y="115" Z="60"/>
            <Vertex X="120" Y="120" Z="60"/>
            <Vertex X="125" Y="115" Z="60"/>
            <Vertex X="130" Y="120" Z="65"/>
          </Wire>
        </Primitives>
      </Metal>
    </Properties>
    <RectilinearGrid>
      <XLines/>
      <YLines/>
      <ZLines/>
    </RectilinearGrid>
  </ContinuousStructure>
</openEMS>
