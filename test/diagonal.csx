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
          <LinPoly Priority="1" Elevation="1" Length="3" NormDir="2">
            <!-- CW -->
            <Vertex X1="10" X2="8" />
            <Vertex X1="10" X2="14" />
            <Vertex X1="11" X2="18" />
            <Vertex X1="13" X2="20" />
            <Vertex X1="17" X2="21" />
            <Vertex X1="21" X2="20" />
            <Vertex X1="23" X2="18" />
            <Vertex X1="24" X2="14" />
            <Vertex X1="31" X2="17" />
            <Vertex X1="32" X2="12" />
            <Vertex X1="36" X2="12" />
            <Vertex X1="36" X2="10" />
            <Vertex X1="27" X2="10" />
            <Vertex X1="27" X2="8" />
          </LinPoly>
          <LinPoly Priority="2" Elevation="1" Length="3" NormDir="2">
            <!-- CCW -->
            <Vertex X1="40" X2="-15" />
            <Vertex X1="41" X2="-15" />
            <Vertex X1="46" X2="-5" />
            <Vertex X1="45" X2="-5" />
          </LinPoly>
          <Box Priority="1">
            <P1 X="35" Y="-9" Z="1">
            </P1>
            <P2 X="50" Y="-11" Z="-1">
            </P2>
          </Box>
          <Box Priority="1">
            <P1 X="0" Y="0" Z="1">
            </P1>
            <P2 X="-20" Y="-20" Z="-1">
            </P2>
          </Box>
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
