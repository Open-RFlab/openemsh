<?xml version="1.0" encoding="UTF-8" standalone="yes" ?>
<openEMS>
  <FDTD NumberOfTimesteps="300000" endCriteria="0.0001" f_max="5000000000">
    <Excitation Type="0" f0="2550000000" fc="2450000000">
    </Excitation>
    <BoundaryCond xmin="PML_8" xmax="PML_8" ymin="PML_8" ymax="PML_8" zmin="PML_8" zmax="PML_8">
    </BoundaryCond>
  </FDTD>
  <ContinuousStructure CoordSystem="0">
    <Properties>
<!--
      <Material Name="Subst1.substrate">
        <Property Epsilon="4.7" Kappa="0.00933457764526183">
        </Property>
        <Primitives>
          <Box Priority="1">
            <P1 X="15.5" Y="-60.3024" Z="-1.55">
            </P1>
            <P2 X="155.883" Y="-15.5" Z="0">
            </P2>
          </Box>
        </Primitives>
      </Material>
      <ConductingSheet Name="Subst1.ground" Conductivity="59594755.6615018" Thickness="3.5e-05">
        <Primitives>
          <Box Priority="2">
            <P1 X="15.5" Y="-60.3024" Z="-1.55">
            </P1>
            <P2 X="155.883" Y="-15.5" Z="-1.55">
            </P2>
          </Box>
        </Primitives>
      </ConductingSheet>
-->
      <ConductingSheet Name="MS1" Conductivity="59594755.6615018" Thickness="3.5e-05">
        <Primitives>
          <Box Priority="2">
            <P1 X="31.1" Y="-44.8024" Z="0">
            </P1>
            <P2 X="51.3539" Y="-42.0164" Z="0">
            </P2>
          </Box>
        </Primitives>
      </ConductingSheet>
      <ConductingSheet Name="MS2" Conductivity="59594755.6615018" Thickness="3.5e-05">
        <Primitives>
          <Box Priority="2">
            <P1 X="120.029" Y="-44.8024" Z="0">
            </P1>
            <P2 X="140.283" Y="-42.0164" Z="0">
            </P2>
          </Box>
        </Primitives>
      </ConductingSheet>
      <ConductingSheet Name="MS3" Conductivity="59594755.6615018" Thickness="3.5e-05">
        <Primitives>
          <Box Priority="2">
            <P1 X="51.6077" Y="-42.0164" Z="0">
            </P1>
            <P2 X="51.3539" Y="-31" Z="0">
            </P2>
          </Box>
        </Primitives>
      </ConductingSheet>
      <ConductingSheet Name="MS4" Conductivity="59594755.6615018" Thickness="3.5e-05">
        <Primitives>
          <Box Priority="2">
            <P1 X="120.024" Y="-42.0164" Z="0">
            </P1>
            <P2 X="119.78" Y="-31" Z="0">
            </P2>
          </Box>
        </Primitives>
      </ConductingSheet>
      <ConductingSheet Name="MS5" Conductivity="59594755.6615018" Thickness="3.5e-05">
        <Primitives>
          <Box Priority="2">
            <P1 X="109.208" Y="-43.9276" Z="0">
            </P1>
            <P2 X="119.775" Y="-42.8913" Z="0">
            </P2>
          </Box>
        </Primitives>
      </ConductingSheet>
      <ConductingSheet Name="MS6" Conductivity="59594755.6615018" Thickness="3.5e-05">
        <Primitives>
          <Box Priority="2">
            <P1 X="51.6077" Y="-43.9276" Z="0">
            </P1>
            <P2 X="62.1753" Y="-42.8913" Z="0">
            </P2>
          </Box>
        </Primitives>
      </ConductingSheet>
      <ConductingSheet Name="MS7" Conductivity="59594755.6615018" Thickness="3.5e-05">
        <Primitives>
          <Box Priority="2">
            <P1 X="70.3673" Y="-42.8674" Z="0">
            </P1>
            <P2 X="62.1753" Y="-33.2952" Z="0">
            </P2>
          </Box>
        </Primitives>
      </ConductingSheet>
      <ConductingSheet Name="MS8" Conductivity="59594755.6615018" Thickness="3.5e-05">
        <Primitives>
          <Box Priority="2">
            <P1 X="109.208" Y="-42.8674" Z="0">
            </P1>
            <P2 X="101.016" Y="-33.2952" Z="0">
            </P2>
          </Box>
        </Primitives>
      </ConductingSheet>
      <ConductingSheet Name="MS9" Conductivity="59594755.6615018" Thickness="3.5e-05">
        <Primitives>
          <Box Priority="2">
            <P1 X="90.96" Y="-42.8674" Z="0">
            </P1>
            <P2 X="80.4229" Y="-33.1334" Z="0">
            </P2>
          </Box>
        </Primitives>
      </ConductingSheet>
      <ConductingSheet Name="MS10" Conductivity="59594755.6615018" Thickness="3.5e-05">
        <Primitives>
          <Box Priority="2">
            <P1 X="70.3673" Y="-43.9514" Z="0">
            </P1>
            <P2 X="80.4229" Y="-42.8674" Z="0">
            </P2>
          </Box>
        </Primitives>
      </ConductingSheet>
      <ConductingSheet Name="MS11" Conductivity="59594755.6615018" Thickness="3.5e-05">
        <Primitives>
          <Box Priority="2">
            <P1 X="90.96" Y="-43.9514" Z="0">
            </P1>
            <P2 X="101.016" Y="-42.8674" Z="0">
            </P2>
          </Box>
        </Primitives>
      </ConductingSheet>
      <ConductingSheet Name="MS12" Conductivity="59594755.6615018" Thickness="3.5e-05">
        <Primitives>
          <LinPoly Priority="2" Elevation="0" Length="0" NormDir="2">
            <Vertex X1="51.6077" X2="-42.0164">
            </Vertex>
            <Vertex X1="51.3539" X2="-42.0164">
            </Vertex>
            <Vertex X1="51.3539" X2="-44.8024">
            </Vertex>
            <Vertex X1="51.4808" X2="-44.8024">
            </Vertex>
            <Vertex X1="51.4808" X2="-43.9276">
            </Vertex>
            <Vertex X1="51.6077" X2="-43.9276">
            </Vertex>
          </LinPoly>
        </Primitives>
      </ConductingSheet>
      <ConductingSheet Name="MS13" Conductivity="59594755.6615018" Thickness="3.5e-05">
        <Primitives>
          <LinPoly Priority="2" Elevation="0" Length="0" NormDir="2">
            <Vertex X1="70.3673" X2="-42.8674">
            </Vertex>
            <Vertex X1="62.1753" X2="-42.8674">
            </Vertex>
            <Vertex X1="62.1753" X2="-43.9276">
            </Vertex>
            <Vertex X1="66.2713" X2="-43.9276">
            </Vertex>
            <Vertex X1="66.2713" X2="-43.9514">
            </Vertex>
            <Vertex X1="70.3673" X2="-43.9514">
            </Vertex>
          </LinPoly>
        </Primitives>
      </ConductingSheet>
      <ConductingSheet Name="MS14" Conductivity="59594755.6615018" Thickness="3.5e-05">
        <Primitives>
          <LinPoly Priority="2" Elevation="0" Length="0" NormDir="2">
            <Vertex X1="90.96" X2="-42.8674">
            </Vertex>
            <Vertex X1="80.4229" X2="-42.8674">
            </Vertex>
            <Vertex X1="80.4229" X2="-43.9514">
            </Vertex>
            <Vertex X1="90.96" X2="-43.9514">
            </Vertex>
          </LinPoly>
        </Primitives>
      </ConductingSheet>
      <ConductingSheet Name="MS15" Conductivity="59594755.6615018" Thickness="3.5e-05">
        <Primitives>
          <LinPoly Priority="2" Elevation="0" Length="0" NormDir="2">
            <Vertex X1="109.208" X2="-42.8674">
            </Vertex>
            <Vertex X1="101.016" X2="-42.8674">
            </Vertex>
            <Vertex X1="101.016" X2="-43.9514">
            </Vertex>
            <Vertex X1="105.112" X2="-43.9514">
            </Vertex>
            <Vertex X1="105.112" X2="-43.9276">
            </Vertex>
            <Vertex X1="109.208" X2="-43.9276">
            </Vertex>
          </LinPoly>
        </Primitives>
      </ConductingSheet>
      <ConductingSheet Name="MS16" Conductivity="59594755.6615018" Thickness="3.5e-05">
        <Primitives>
          <LinPoly Priority="2" Elevation="0" Length="0" NormDir="2">
            <Vertex X1="120.029" X2="-42.0164">
            </Vertex>
            <Vertex X1="119.775" X2="-42.0164">
            </Vertex>
            <Vertex X1="119.775" X2="-43.9276">
            </Vertex>
            <Vertex X1="119.902" X2="-43.9276">
            </Vertex>
            <Vertex X1="119.902" X2="-44.8024">
            </Vertex>
            <Vertex X1="120.029" X2="-44.8024">
            </Vertex>
          </LinPoly>
        </Primitives>
      </ConductingSheet>
      <LumpedElement Name="port_resist_1" Direction="2" Caps="1" R="50">
        <Primitives>
          <Box Priority="5">
            <P1 X="31" Y="-44.8024" Z="-1.55">
            </P1>
            <P2 X="31.2" Y="-42.0164" Z="0">
            </P2>
          </Box>
        </Primitives>
      </LumpedElement>
      <LumpedElement Name="port_resist_2" Direction="2" Caps="1" R="50">
        <Primitives>
          <Box Priority="5">
            <P1 X="140.183" Y="-44.8024" Z="-1.55">
            </P1>
            <P2 X="140.383" Y="-42.0164" Z="0">
            </P2>
          </Box>
        </Primitives>
      </LumpedElement>
      <Excitation Name="port_excite_1" Type="0" Excite="-0,-0,-1">
        <Primitives>
          <Box Priority="5">
            <P1 X="31" Y="-44.8024" Z="-1.55">
            </P1>
            <P2 X="31.2" Y="-42.0164" Z="0">
            </P2>
          </Box>
        </Primitives>
      </Excitation>
      <ProbeBox Name="port_ut1" Type="0" Weight="-1">
        <Primitives>
          <Box Priority="5">
            <P1 X="31.1" Y="-43.4094" Z="-1.55">
            </P1>
            <P2 X="31.1" Y="-43.4094" Z="0">
            </P2>
          </Box>
        </Primitives>
      </ProbeBox>
      <ProbeBox Name="port_it1" Type="1" Weight="1" NormDir="2">
        <Primitives>
          <Box Priority="5">
            <P1 X="31" Y="-44.8024" Z="-0.775">
            </P1>
            <P2 X="31.2" Y="-42.0164" Z="-0.775">
            </P2>
          </Box>
        </Primitives>
      </ProbeBox>
      <ProbeBox Name="port_ut2" Type="0" Weight="-1">
        <Primitives>
          <Box Priority="5">
            <P1 X="140.283" Y="-43.4094" Z="-1.55">
            </P1>
            <P2 X="140.283" Y="-43.4094" Z="0">
            </P2>
          </Box>
        </Primitives>
      </ProbeBox>
      <ProbeBox Name="port_it2" Type="1" Weight="1" NormDir="2">
        <Primitives>
          <Box Priority="5">
            <P1 X="140.183" Y="-44.8024" Z="-0.775">
            </P1>
            <P2 X="140.383" Y="-42.0164" Z="-0.775">
            </P2>
          </Box>
        </Primitives>
      </ProbeBox>
    </Properties>
    <RectilinearGrid>
      <XLines/>
      <YLines/>
      <ZLines/>
    </RectilinearGrid>
  </ContinuousStructure>
</openEMS>
