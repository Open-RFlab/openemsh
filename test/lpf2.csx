<?xml version="1.0"?>
<openEMS>
  <FDTD NumberOfTimesteps="300000" endCriteria="0.0001" f_max="5000000000">
    <Excitation Type="0" f0="2510000000" fc="2490000000" />
    <BoundaryCond xmin="PML_8" xmax="PML_8" ymin="PML_8" ymax="PML_8" zmin="PML_8" zmax="PML_8" />
  </FDTD>
  <ContinuousStructure CoordSystem="0">
    <Properties>
      <Material Name="Sub1.substrate">
        <Property Epsilon="4.2" Kappa="0.0205267245591052" />
        <Primitives>
          <Box Priority="1">
            <P1 X="8" Y="-44.5" Z="-0.8" />
            <P2 X="74.7" Y="-8" Z="0" />
          </Box>
        </Primitives>
      </Material>
      <ConductingSheet Name="MS1" Conductivity="10000000000" Thickness="3.5e-05">
        <Primitives>
          <Box Priority="2">
            <P1 X="16.1" Y="-26.5" Z="0" />
            <P2 X="20.6" Y="-26" Z="0" />
          </Box>
        </Primitives>
      </ConductingSheet>
      <ConductingSheet Name="MS2" Conductivity="10000000000" Thickness="3.5e-05">
        <Primitives>
          <Box Priority="2">
            <P1 X="20.6" Y="-36.5" Z="0" />
            <P2 X="22.1" Y="-16" Z="0" />
          </Box>
        </Primitives>
      </ConductingSheet>
      <ConductingSheet Name="MS3" Conductivity="10000000000" Thickness="3.5e-05">
        <Primitives>
          <Box Priority="2">
            <P1 X="22.1" Y="-26.5" Z="0" />
            <P2 X="40.1" Y="-26" Z="0" />
          </Box>
        </Primitives>
      </ConductingSheet>
      <ConductingSheet Name="MS4" Conductivity="10000000000" Thickness="3.5e-05">
        <Primitives>
          <Box Priority="2">
            <P1 X="40.1" Y="-36.5" Z="0" />
            <P2 X="42.6" Y="-16" Z="0" />
          </Box>
        </Primitives>
      </ConductingSheet>
      <ConductingSheet Name="MS5" Conductivity="10000000000" Thickness="3.5e-05">
        <Primitives>
          <Box Priority="2">
            <P1 X="42.6" Y="-26.5" Z="0" />
            <P2 X="60.6" Y="-26" Z="0" />
          </Box>
        </Primitives>
      </ConductingSheet>
      <ConductingSheet Name="MS6" Conductivity="10000000000" Thickness="3.5e-05">
        <Primitives>
          <Box Priority="2">
            <P1 X="60.6" Y="-36.5" Z="0" />
            <P2 X="62.1" Y="-16" Z="0" />
          </Box>
        </Primitives>
      </ConductingSheet>
      <ConductingSheet Name="MS7" Conductivity="10000000000" Thickness="3.5e-05">
        <Primitives>
          <Box Priority="2">
            <P1 X="62.1" Y="-26.5" Z="0" />
            <P2 X="66.6" Y="-26" Z="0" />
          </Box>
        </Primitives>
      </ConductingSheet>
      <LumpedElement Name="port_resist_1" Direction="2" Caps="1" R="50">
        <Primitives>
          <Box Priority="5">
            <P1 X="16" Y="-26.5" Z="-0.8" />
            <P2 X="16.2" Y="-26" Z="0" />
          </Box>
        </Primitives>
      </LumpedElement>
      <LumpedElement Name="port_resist_2" Direction="2" Caps="1" R="50">
        <Primitives>
          <Box Priority="5">
            <P1 X="66.5" Y="-26.5" Z="-0.8" />
            <P2 X="66.7" Y="-26" Z="0" />
          </Box>
        </Primitives>
      </LumpedElement>
      <Excitation Name="port_excite_1" Type="0" Excite="-0,-0,-1">
        <Primitives>
          <Box Priority="5">
            <P1 X="16" Y="-26.5" Z="-0.8" />
            <P2 X="16.2" Y="-26" Z="0" />
          </Box>
        </Primitives>
      </Excitation>
      <ProbeBox Name="port_ut1" Type="0" Weight="-1">
        <Primitives>
          <Box Priority="5">
            <P1 X="16.1" Y="-26.25" Z="-0.8" />
            <P2 X="16.1" Y="-26.25" Z="0" />
          </Box>
        </Primitives>
      </ProbeBox>
      <ProbeBox Name="port_it1" Type="1" Weight="1" NormDir="2">
        <Primitives>
          <Box Priority="5">
            <P1 X="16" Y="-26.5" Z="-0.4" />
            <P2 X="16.2" Y="-26" Z="-0.4" />
          </Box>
        </Primitives>
      </ProbeBox>
      <ProbeBox Name="port_ut2" Type="0" Weight="-1">
        <Primitives>
          <Box Priority="5">
            <P1 X="66.6" Y="-26.25" Z="-0.8" />
            <P2 X="66.6" Y="-26.25" Z="0" />
          </Box>
        </Primitives>
      </ProbeBox>
      <ProbeBox Name="port_it2" Type="1" Weight="1" NormDir="2">
        <Primitives>
          <Box Priority="5">
            <P1 X="66.5" Y="-26.5" Z="-0.4" />
            <P2 X="66.7" Y="-26" Z="-0.4" />
          </Box>
        </Primitives>
      </ProbeBox>
      <DumpBox Name="nf2ff_E_xn" DumpMode="1" DumpType="0" FileType="1">
        <Primitives>
          <Box Priority="0">
            <P1 X="-11.1197431665909" Y="-63.9582637668182" Z="-27.9093535428571" />
            <P2 X="-11.1197431665909" Y="11.4582637668182" Z="27.1093535428571" />
          </Box>
        </Primitives>
      </DumpBox>
      <DumpBox Name="nf2ff_H_xn" DumpMode="1" DumpType="1" FileType="1">
        <Primitives>
          <Box Priority="0">
            <P1 X="-11.1197431665909" Y="-63.9582637668182" Z="-27.9093535428571" />
            <P2 X="-11.1197431665909" Y="11.4582637668182" Z="27.1093535428571" />
          </Box>
        </Primitives>
      </DumpBox>
      <DumpBox Name="nf2ff_E_xp" DumpMode="1" DumpType="0" FileType="1">
        <Primitives>
          <Box Priority="0">
            <P1 X="93.8197431665909" Y="-63.9582637668182" Z="-27.9093535428571" />
            <P2 X="93.8197431665909" Y="11.4582637668182" Z="27.1093535428571" />
          </Box>
        </Primitives>
      </DumpBox>
      <DumpBox Name="nf2ff_H_xp" DumpMode="1" DumpType="1" FileType="1">
        <Primitives>
          <Box Priority="0">
            <P1 X="93.8197431665909" Y="-63.9582637668182" Z="-27.9093535428571" />
            <P2 X="93.8197431665909" Y="11.4582637668182" Z="27.1093535428571" />
          </Box>
        </Primitives>
      </DumpBox>
      <DumpBox Name="nf2ff_E_yn" DumpMode="1" DumpType="0" FileType="1">
        <Primitives>
          <Box Priority="0">
            <P1 X="-11.1197431665909" Y="-63.9582637668182" Z="-27.9093535428571" />
            <P2 X="93.8197431665909" Y="-63.9582637668182" Z="27.1093535428571" />
          </Box>
        </Primitives>
      </DumpBox>
      <DumpBox Name="nf2ff_H_yn" DumpMode="1" DumpType="1" FileType="1">
        <Primitives>
          <Box Priority="0">
            <P1 X="-11.1197431665909" Y="-63.9582637668182" Z="-27.9093535428571" />
            <P2 X="93.8197431665909" Y="-63.9582637668182" Z="27.1093535428571" />
          </Box>
        </Primitives>
      </DumpBox>
      <DumpBox Name="nf2ff_E_yp" DumpMode="1" DumpType="0" FileType="1">
        <Primitives>
          <Box Priority="0">
            <P1 X="-11.1197431665909" Y="11.4582637668182" Z="-27.9093535428571" />
            <P2 X="93.8197431665909" Y="11.4582637668182" Z="27.1093535428571" />
          </Box>
        </Primitives>
      </DumpBox>
      <DumpBox Name="nf2ff_H_yp" DumpMode="1" DumpType="1" FileType="1">
        <Primitives>
          <Box Priority="0">
            <P1 X="-11.1197431665909" Y="11.4582637668182" Z="-27.9093535428571" />
            <P2 X="93.8197431665909" Y="11.4582637668182" Z="27.1093535428571" />
          </Box>
        </Primitives>
      </DumpBox>
      <DumpBox Name="nf2ff_E_zn" DumpMode="1" DumpType="0" FileType="1">
        <Primitives>
          <Box Priority="0">
            <P1 X="-11.1197431665909" Y="-63.9582637668182" Z="-27.9093535428571" />
            <P2 X="93.8197431665909" Y="11.4582637668182" Z="-27.9093535428571" />
          </Box>
        </Primitives>
      </DumpBox>
      <DumpBox Name="nf2ff_H_zn" DumpMode="1" DumpType="1" FileType="1">
        <Primitives>
          <Box Priority="0">
            <P1 X="-11.1197431665909" Y="-63.9582637668182" Z="-27.9093535428571" />
            <P2 X="93.8197431665909" Y="11.4582637668182" Z="-27.9093535428571" />
          </Box>
        </Primitives>
      </DumpBox>
      <DumpBox Name="nf2ff_E_zp" DumpMode="1" DumpType="0" FileType="1">
        <Primitives>
          <Box Priority="0">
            <P1 X="-11.1197431665909" Y="-63.9582637668182" Z="27.1093535428571" />
            <P2 X="93.8197431665909" Y="11.4582637668182" Z="27.1093535428571" />
          </Box>
        </Primitives>
      </DumpBox>
      <DumpBox Name="nf2ff_H_zp" DumpMode="1" DumpType="1" FileType="1">
        <Primitives>
          <Box Priority="0">
            <P1 X="-11.1197431665909" Y="-63.9582637668182" Z="27.1093535428571" />
            <P2 X="93.8197431665909" Y="11.4582637668182" Z="27.1093535428571" />
          </Box>
        </Primitives>
      </DumpBox>
    </Properties>
    <RectilinearGrid DeltaUnit="1e-03">
      <YLines>-44.498696,-44.491457,-44.478059,-44.453263,-44.407376,-44.322454,-44.165293,-43.874440,-43.336169,-42.340012,-40.500000,-38.659130,-37.663970,-37.126237,-36.835675,-36.678671,-36.593834,-36.547992,-36.523222,-36.509838,-36.502605,-36.498697,-36.491280,-36.477204,-36.450488,-36.399785,-36.303555,-36.120922,-35.774303,-35.116457,-33.867935,-31.500000,-29.093254,-27.844396,-27.196882,-26.861157,-26.687088,-26.596836,-26.550042,-26.525780,-26.513201,-26.506678,-26.503297,-26.501543,-26.500634,-26.500163,-26.499919,-26.499466,-26.498629,-26.497080,-26.494214,-26.488908,-26.479090,-26.460919,-26.427292,-26.365059,-26.250000,-26.134941,-26.072708,-26.039081,-26.020910,-26.011092,-26.005786,-26.002920,-26.001371,-26.000534,-26.000081,-25.999837,-25.999366,-25.998457,-25.996703,-25.993322,-25.986799,-25.974220,-25.949958,-25.903164,-25.812912,-25.638843,-25.303118,-24.655604,-23.406746,-21.000000,-18.632065,-17.383543,-16.725697,-16.379078,-16.196445,-16.100215,-16.049512,-16.022796,-16.008720,-16.001303,-15.997395,-15.990162,-15.976778,-15.952008,-15.906166,-15.821329,-15.664325,-15.373763,-14.836030,-13.840870,-12.000000,-10.159988,-9.163831,-8.625560,-8.334707,-8.177546,-8.092624,-8.046737,-8.021941,-8.008543,-8.001304</YLines>
    </RectilinearGrid>
  </ContinuousStructure>
</openEMS>
