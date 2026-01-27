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
          </Box>
          <Point Priority="1" X="1" Y="1" Z="1"/>
          <LinPoly Priority="1" Elevation="1" Length="3" NormDir="2">
            <Vertex X1="10" X2="10"/>
            <Vertex X1="10" X2="20"/>
            <Vertex X1="30" X2="20"/>
          </LinPoly>
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
