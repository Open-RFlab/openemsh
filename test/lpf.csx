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
<!--
      <Material Name="Sub1.substrate">
        <Property Epsilon="4.2" Kappa="0.0205267245591052">
        </Property>
        <Primitives>
          <Box Priority="1">
            <P1 X="8" Y="-44.5" Z="-0.8">
            </P1>
            <P2 X="74.7" Y="-8" Z="0">
            </P2>
          </Box>
        </Primitives>
      </Material>
      <ConductingSheet Name="Sub1.ground" Conductivity="10000000000" Thickness="3.5e-05">
        <Primitives>
          <Box Priority="2">
            <P1 X="8" Y="-44.5" Z="-0.8">
            </P1>
            <P2 X="74.7" Y="-8" Z="-0.8">
            </P2>
          </Box>
        </Primitives>
      </ConductingSheet>
-->
      <ConductingSheet Name="MS1" Conductivity="10000000000" Thickness="3.5e-05">
        <Primitives>
          <Box Priority="2">
            <P1 X="16.1" Y="-26.5" Z="0">
            </P1>
            <P2 X="20.6" Y="-26" Z="0">
            </P2>
          </Box>
        </Primitives>
      </ConductingSheet>
      <ConductingSheet Name="MS2" Conductivity="10000000000" Thickness="3.5e-05">
        <Primitives>
          <Box Priority="2">
            <P1 X="20.6" Y="-36.5" Z="0">
            </P1>
            <P2 X="22.1" Y="-16" Z="0">
            </P2>
          </Box>
        </Primitives>
      </ConductingSheet>
      <ConductingSheet Name="MS3" Conductivity="10000000000" Thickness="3.5e-05">
        <Primitives>
          <Box Priority="2">
            <P1 X="22.1" Y="-26.5" Z="0">
            </P1>
            <P2 X="40.1" Y="-26" Z="0">
            </P2>
          </Box>
        </Primitives>
      </ConductingSheet>
      <ConductingSheet Name="MS4" Conductivity="10000000000" Thickness="3.5e-05">
        <Primitives>
          <Box Priority="2">
            <P1 X="40.1" Y="-36.5" Z="0">
            </P1>
            <P2 X="42.6" Y="-16" Z="0">
            </P2>
          </Box>
        </Primitives>
      </ConductingSheet>
      <ConductingSheet Name="MS5" Conductivity="10000000000" Thickness="3.5e-05">
        <Primitives>
          <Box Priority="2">
            <P1 X="42.6" Y="-26.5" Z="0">
            </P1>
            <P2 X="60.6" Y="-26" Z="0">
            </P2>
          </Box>
        </Primitives>
      </ConductingSheet>
      <ConductingSheet Name="MS6" Conductivity="10000000000" Thickness="3.5e-05">
        <Primitives>
          <Box Priority="2">
            <P1 X="60.6" Y="-36.5" Z="0">
            </P1>
            <P2 X="62.1" Y="-16" Z="0">
            </P2>
          </Box>
        </Primitives>
      </ConductingSheet>
      <ConductingSheet Name="MS7" Conductivity="10000000000" Thickness="3.5e-05">
        <Primitives>
          <Box Priority="2">
            <P1 X="62.1" Y="-26.5" Z="0">
            </P1>
            <P2 X="66.6" Y="-26" Z="0">
            </P2>
          </Box>
        </Primitives>
      </ConductingSheet>
      <LumpedElement Name="port_resist_1" Direction="2" Caps="1" R="50">
        <Primitives>
          <Box Priority="5">
            <P1 X="16" Y="-26.5" Z="-0.8">
            </P1>
            <P2 X="16.2" Y="-26" Z="0">
            </P2>
          </Box>
        </Primitives>
      </LumpedElement>
      <LumpedElement Name="port_resist_2" Direction="2" Caps="1" R="50">
        <Primitives>
          <Box Priority="5">
            <P1 X="66.5" Y="-26.5" Z="-0.8">
            </P1>
            <P2 X="66.7" Y="-26" Z="0">
            </P2>
          </Box>
        </Primitives>
      </LumpedElement>
      <Excitation Name="port_excite_1" Type="0" Excite="-0,-0,-1">
        <Primitives>
          <Box Priority="5">
            <P1 X="16" Y="-26.5" Z="-0.8">
            </P1>
            <P2 X="16.2" Y="-26" Z="0">
            </P2>
          </Box>
        </Primitives>
      </Excitation>
      <ProbeBox Name="port_ut1" Type="0" Weight="-1">
        <Primitives>
          <Box Priority="5">
            <P1 X="16.1" Y="-26.25" Z="-0.8">
            </P1>
            <P2 X="16.1" Y="-26.25" Z="0">
            </P2>
          </Box>
        </Primitives>
      </ProbeBox>
      <ProbeBox Name="port_it1" Type="1" Weight="1" NormDir="2">
        <Primitives>
          <Box Priority="5">
            <P1 X="16" Y="-26.5" Z="-0.4">
            </P1>
            <P2 X="16.2" Y="-26" Z="-0.4">
            </P2>
          </Box>
        </Primitives>
      </ProbeBox>
      <ProbeBox Name="port_ut2" Type="0" Weight="-1">
        <Primitives>
          <Box Priority="5">
            <P1 X="66.6" Y="-26.25" Z="-0.8">
            </P1>
            <P2 X="66.6" Y="-26.25" Z="0">
            </P2>
          </Box>
        </Primitives>
      </ProbeBox>
      <ProbeBox Name="port_it2" Type="1" Weight="1" NormDir="2">
        <Primitives>
          <Box Priority="5">
            <P1 X="66.5" Y="-26.5" Z="-0.4">
            </P1>
            <P2 X="66.7" Y="-26" Z="-0.4">
            </P2>
          </Box>
        </Primitives>
      </ProbeBox>
      <DumpBox Name="nf2ff_E_xn" DumpMode="1" DumpType="0" FileType="1">
        <Primitives>
          <Box Priority="0">
            <P1 X="-11.1197431665909" Y="-63.9582637668182" Z="-27.9093535428571">
            </P1>
            <P2 X="-11.1197431665909" Y="11.4582637668182" Z="27.1093535428571">
            </P2>
          </Box>
        </Primitives>
      </DumpBox>
      <DumpBox Name="nf2ff_H_xn" DumpMode="1" DumpType="1" FileType="1">
        <Primitives>
          <Box Priority="0">
            <P1 X="-11.1197431665909" Y="-63.9582637668182" Z="-27.9093535428571">
            </P1>
            <P2 X="-11.1197431665909" Y="11.4582637668182" Z="27.1093535428571">
            </P2>
          </Box>
        </Primitives>
      </DumpBox>
      <DumpBox Name="nf2ff_E_xp" DumpMode="1" DumpType="0" FileType="1">
        <Primitives>
          <Box Priority="0">
            <P1 X="93.8197431665909" Y="-63.9582637668182" Z="-27.9093535428571">
            </P1>
            <P2 X="93.8197431665909" Y="11.4582637668182" Z="27.1093535428571">
            </P2>
          </Box>
        </Primitives>
      </DumpBox>
      <DumpBox Name="nf2ff_H_xp" DumpMode="1" DumpType="1" FileType="1">
        <Primitives>
          <Box Priority="0">
            <P1 X="93.8197431665909" Y="-63.9582637668182" Z="-27.9093535428571">
            </P1>
            <P2 X="93.8197431665909" Y="11.4582637668182" Z="27.1093535428571">
            </P2>
          </Box>
        </Primitives>
      </DumpBox>
      <DumpBox Name="nf2ff_E_yn" DumpMode="1" DumpType="0" FileType="1">
        <Primitives>
          <Box Priority="0">
            <P1 X="-11.1197431665909" Y="-63.9582637668182" Z="-27.9093535428571">
            </P1>
            <P2 X="93.8197431665909" Y="-63.9582637668182" Z="27.1093535428571">
            </P2>
          </Box>
        </Primitives>
      </DumpBox>
      <DumpBox Name="nf2ff_H_yn" DumpMode="1" DumpType="1" FileType="1">
        <Primitives>
          <Box Priority="0">
            <P1 X="-11.1197431665909" Y="-63.9582637668182" Z="-27.9093535428571">
            </P1>
            <P2 X="93.8197431665909" Y="-63.9582637668182" Z="27.1093535428571">
            </P2>
          </Box>
        </Primitives>
      </DumpBox>
      <DumpBox Name="nf2ff_E_yp" DumpMode="1" DumpType="0" FileType="1">
        <Primitives>
          <Box Priority="0">
            <P1 X="-11.1197431665909" Y="11.4582637668182" Z="-27.9093535428571">
            </P1>
            <P2 X="93.8197431665909" Y="11.4582637668182" Z="27.1093535428571">
            </P2>
          </Box>
        </Primitives>
      </DumpBox>
      <DumpBox Name="nf2ff_H_yp" DumpMode="1" DumpType="1" FileType="1">
        <Primitives>
          <Box Priority="0">
            <P1 X="-11.1197431665909" Y="11.4582637668182" Z="-27.9093535428571">
            </P1>
            <P2 X="93.8197431665909" Y="11.4582637668182" Z="27.1093535428571">
            </P2>
          </Box>
        </Primitives>
      </DumpBox>
      <DumpBox Name="nf2ff_E_zn" DumpMode="1" DumpType="0" FileType="1">
        <Primitives>
          <Box Priority="0">
            <P1 X="-11.1197431665909" Y="-63.9582637668182" Z="-27.9093535428571">
            </P1>
            <P2 X="93.8197431665909" Y="11.4582637668182" Z="-27.9093535428571">
            </P2>
          </Box>
        </Primitives>
      </DumpBox>
      <DumpBox Name="nf2ff_H_zn" DumpMode="1" DumpType="1" FileType="1">
        <Primitives>
          <Box Priority="0">
            <P1 X="-11.1197431665909" Y="-63.9582637668182" Z="-27.9093535428571">
            </P1>
            <P2 X="93.8197431665909" Y="11.4582637668182" Z="-27.9093535428571">
            </P2>
          </Box>
        </Primitives>
      </DumpBox>
      <DumpBox Name="nf2ff_E_zp" DumpMode="1" DumpType="0" FileType="1">
        <Primitives>
          <Box Priority="0">
            <P1 X="-11.1197431665909" Y="-63.9582637668182" Z="27.1093535428571">
            </P1>
            <P2 X="93.8197431665909" Y="11.4582637668182" Z="27.1093535428571">
            </P2>
          </Box>
        </Primitives>
      </DumpBox>
      <DumpBox Name="nf2ff_H_zp" DumpMode="1" DumpType="1" FileType="1">
        <Primitives>
          <Box Priority="0">
            <P1 X="-11.1197431665909" Y="-63.9582637668182" Z="27.1093535428571">
            </P1>
            <P2 X="93.8197431665909" Y="11.4582637668182" Z="27.1093535428571">
            </P2>
          </Box>
        </Primitives>
      </DumpBox>
    </Properties>
    <RectilinearGrid>
      <XLines/>
      <YLines/>
      <ZLines/>
    </RectilinearGrid>
  </ContinuousStructure>
</openEMS>
