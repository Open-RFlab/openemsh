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
            <P1 X="0" Y="0" Z="1"/>
            <P2 X="-20" Y="-20" Z="-1"/>
          </Box>
          <LinPoly Priority="1" Elevation="1" Length="3" NormDir="2">
            <Vertex X1="10" X2="10" />
            <Vertex X1="10" X2="20" />
            <Vertex X1="30" X2="20" />
          </LinPoly>
          <Sphere Priority="1" Radius="5">
            <Center X="50" Y="50" Z="10"/>
          </Sphere>
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
