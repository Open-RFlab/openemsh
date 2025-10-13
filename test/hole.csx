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
      <Material Name="Hole">
        <Primitives>
          <Box Priority="2">
            <P1 X="5" Y="5" Z="2">
            </P1>
            <P2 X="10" Y="10" Z="-2">
            </P2>
          </Box>
          <Box Priority="0">
            <P1 X="16" Y="16" Z="2">
            </P1>
            <P2 X="18" Y="18" Z="-2">
            </P2>
          </Box>
          <Box Priority="0">
            <P1 X="21" Y="21" Z="1">
            </P1>
            <P2 X="23" Y="23" Z="-1">
            </P2>
          </Box>
        </Primitives>
      </Material>
      <Metal Name="Shape">
        <Primitives>
          <Box Priority="1">
            <P1 X="0" Y="0" Z="1">
            </P1>
            <P2 X="25" Y="25" Z="-1">
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
