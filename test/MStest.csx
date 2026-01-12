<?xml version="1.0"?>
<OpenEMSH>
  <GlobalParams ProximityLimit="0.10000000000000001" Smoothness="2" dmax="500" lmin="2" nmin="20" />
</OpenEMSH>
<openEMS>
  <FDTD NumberOfTimesteps="300000" endCriteria="0.0001" f_max="5000000000">
    <Excitation Type="0" f0="2510000000" fc="2490000000" />
    <BoundaryCond xmin="PML_8" xmax="PML_8" ymin="PML_8" ymax="PML_8" zmin="PML_8" zmax="PML_8" />
  </FDTD>
  <ContinuousStructure CoordSystem="0">
    <Properties>
      <Material Name="Subst1.substrate">
        <Property Epsilon="9.8" Kappa="0.000273689660788069" />
        <Primitives>
          <Box Priority="1">
            <P1 X="10" Y="-108.856" Z="-1" />
            <P2 X="80.7114" Y="-10" Z="0" />
          </Box>
        </Primitives>
      </Material>
      <ConductingSheet Name="Subst1.ground" Conductivity="45454545.4545455" Thickness="3.5e-05">
        <Primitives>
          <Box Priority="2">
            <P1 X="10" Y="-108.856" Z="-1" />
            <P2 X="80.7114" Y="-10" Z="-1" />
          </Box>
        </Primitives>
      </ConductingSheet>
      <ConductingSheet Name="MS25" Conductivity="45454545.4545455" Thickness="3.5e-05">
        <Primitives>
          <Box Priority="2">
            <P1 X="29.5774" Y="-29.5631" Z="0" />
            <P2 X="39.5774" Y="-28.5631" Z="0" />
          </Box>
        </Primitives>
      </ConductingSheet>
      <ConductingSheet Name="MS26" Conductivity="45454545.4545455" Thickness="3.5e-05">
        <Primitives>
          <LinPoly Priority="2" Elevation="0" Length="0" NormDir="2">
            <Vertex X1="29.5774" X2="-28.1568" />
            <Vertex X1="29.5774" X2="-29.9694" />
            <Vertex X1="25.7738" X2="-38.1262" />
            <!--Vertex X1="25.7738" X2="-38.1262" /-->
            <Vertex X1="25.3503" X2="-37.9163" />
            <Vertex X1="24.9372" X2="-37.6867" />
            <Vertex X1="24.5353" X2="-37.4379" />
            <Vertex X1="24.1457" X2="-37.1703" />
            <Vertex X1="23.7692" X2="-36.8846" />
            <Vertex X1="23.4065" X2="-36.5815" />
            <Vertex X1="23.0586" X2="-36.2615" />
            <Vertex X1="22.7263" X2="-35.9255" />
            <Vertex X1="22.4101" X2="-35.5741" />
            <Vertex X1="22.1109" X2="-35.2082" />
            <Vertex X1="21.8294" X2="-34.8286" />
            <Vertex X1="21.5661" X2="-34.4361" />
            <Vertex X1="21.3216" X2="-34.0316" />
            <Vertex X1="21.0965" X2="-33.6159" />
            <Vertex X1="20.8914" X2="-33.1901" />
            <Vertex X1="20.7065" X2="-32.7551" />
            <Vertex X1="20.5424" X2="-32.3119" />
            <Vertex X1="20.3995" X2="-31.8614" />
            <Vertex X1="20.278" X2="-31.4046" />
            <Vertex X1="20.1782" X2="-30.9426" />
            <Vertex X1="20.1004" X2="-30.4764" />
            <Vertex X1="20.0447" X2="-30.0071" />
            <Vertex X1="20.0112" X2="-29.5356" />
            <Vertex X1="20" X2="-29.0631" />
            <Vertex X1="20.0112" X2="-28.5906" />
            <Vertex X1="20.0447" X2="-28.1191" />
            <Vertex X1="20.1004" X2="-27.6497" />
            <Vertex X1="20.1782" X2="-27.1836" />
            <Vertex X1="20.278" X2="-26.7216" />
            <Vertex X1="20.3995" X2="-26.2648" />
            <Vertex X1="20.5424" X2="-25.8143" />
            <Vertex X1="20.7065" X2="-25.371" />
            <Vertex X1="20.8914" X2="-24.936" />
            <Vertex X1="21.0965" X2="-24.5102" />
            <Vertex X1="21.3216" X2="-24.0946" />
            <Vertex X1="21.5661" X2="-23.6901" />
            <Vertex X1="21.8294" X2="-23.2976" />
            <Vertex X1="22.1109" X2="-22.9179" />
            <Vertex X1="22.4101" X2="-22.552" />
            <Vertex X1="22.7263" X2="-22.2007" />
            <Vertex X1="23.0586" X2="-21.8646" />
            <Vertex X1="23.4065" X2="-21.5447" />
            <Vertex X1="23.7692" X2="-21.2415" />
            <Vertex X1="24.1457" X2="-20.9558" />
            <Vertex X1="24.5353" X2="-20.6883" />
            <Vertex X1="24.9372" X2="-20.4394" />
            <Vertex X1="25.3503" X2="-20.2098" />
            <Vertex X1="25.7738" X2="-20" />
            <!--Vertex X1="25.7738" X2="-20" /-->
          </LinPoly>
        </Primitives>
      </ConductingSheet>
      <ConductingSheet Name="MS27" Conductivity="45454545.4545455" Thickness="3.5e-05">
        <Primitives>
          <LinPoly Priority="2" Elevation="0" Length="0" NormDir="2">
            <Vertex X1="39.5774" X2="-29.5631" />
            <Vertex X1="40.5774" X2="-29.5631" />
            <Vertex X1="39.5774" X2="-28.5631" />
          </LinPoly>
        </Primitives>
      </ConductingSheet>
      <ConductingSheet Name="MS28" Conductivity="45454545.4545455" Thickness="3.5e-05">
        <Primitives>
          <LinPoly Priority="2" Elevation="0" Length="0" NormDir="2">
            <Vertex X1="40.5774" X2="-39.5631" />
            <Vertex X1="39.5774" X2="-39.5631" />
            <Vertex X1="40.5774" X2="-40.5631" />
          </LinPoly>
        </Primitives>
      </ConductingSheet>
      <ConductingSheet Name="MS30" Conductivity="45454545.4545455" Thickness="3.5e-05">
        <Primitives>
          <Box Priority="2">
            <P1 X="40.5774" Y="-39.5631" Z="0" />
            <P2 X="39.5774" Y="-29.5631" Z="0" />
          </Box>
        </Primitives>
      </ConductingSheet>
      <ConductingSheet Name="MS31" Conductivity="45454545.4545455" Thickness="3.5e-05">
        <Primitives>
          <LinPoly Priority="2" Elevation="0" Length="0" NormDir="2">
            <Vertex X1="50.5774" X2="-39.5631" />
            <Vertex X1="50.5774" X2="-40.5631" />
            <Vertex X1="51.5774" X2="-39.5631" />
          </LinPoly>
        </Primitives>
      </ConductingSheet>
      <ConductingSheet Name="MS32" Conductivity="45454545.4545455" Thickness="3.5e-05">
        <Primitives>
          <Box Priority="2">
            <P1 X="51.5774" Y="-39.5631" Z="0" />
            <P2 X="50.5774" Y="-29.5631" Z="0" />
          </Box>
        </Primitives>
      </ConductingSheet>
      <ConductingSheet Name="MS33" Conductivity="45454545.4545455" Thickness="3.5e-05">
        <Primitives>
          <LinPoly Priority="2" Elevation="0" Length="0" NormDir="2">
            <Vertex X1="51.5774" X2="-29.5631" />
            <Vertex X1="50.5774" X2="-29.5631" />
            <Vertex X1="51.5774" X2="-28.5631" />
          </LinPoly>
        </Primitives>
      </ConductingSheet>
      <ConductingSheet Name="MS34" Conductivity="45454545.4545455" Thickness="3.5e-05">
        <Primitives>
          <Box Priority="2">
            <P1 X="51.5774" Y="-29.5631" Z="0" />
            <P2 X="61.5774" Y="-28.5631" Z="0" />
          </Box>
        </Primitives>
      </ConductingSheet>
      <ConductingSheet Name="MS35" Conductivity="45454545.4545455" Thickness="3.5e-05">
        <Primitives>
          <LinPoly Priority="2" Elevation="0" Length="0" NormDir="2">
            <Vertex X1="61.5774" X2="-29.5631" />
            <Vertex X1="61.5774" X2="-28.5631" />
            <Vertex X1="69.3716" X2="-24.0631" />
            <!--Vertex X1="69.3716" X2="-24.0631" /-->
            <Vertex X1="69.4786" X2="-24.2532" />
            <Vertex X1="69.5815" X2="-24.4456" />
            <Vertex X1="69.6801" X2="-24.6402" />
            <Vertex X1="69.7744" X2="-24.8369" />
            <Vertex X1="69.8645" X2="-25.0356" />
            <Vertex X1="69.9502" X2="-25.2362" />
            <Vertex X1="70.0314" X2="-25.4387" />
            <Vertex X1="70.1083" X2="-25.6429" />
            <Vertex X1="70.1807" X2="-25.8487" />
            <Vertex X1="70.2485" X2="-26.056" />
            <Vertex X1="70.3119" X2="-26.2648" />
            <Vertex X1="70.3706" X2="-26.4749" />
            <Vertex X1="70.4248" X2="-26.6862" />
            <Vertex X1="70.4743" X2="-26.8987" />
            <Vertex X1="70.5192" X2="-27.1122" />
            <Vertex X1="70.5594" X2="-27.3266" />
            <Vertex X1="70.595" X2="-27.5418" />
            <Vertex X1="70.6258" X2="-27.7578" />
            <Vertex X1="70.6519" X2="-27.9744" />
            <Vertex X1="70.6733" X2="-28.1915" />
            <Vertex X1="70.6899" X2="-28.409" />
            <Vertex X1="70.7018" X2="-28.6269" />
            <Vertex X1="70.709" X2="-28.8449" />
            <Vertex X1="70.7114" X2="-29.0631" />
            <Vertex X1="70.709" X2="-29.2812" />
            <Vertex X1="70.7018" X2="-29.4993" />
            <Vertex X1="70.6899" X2="-29.7171" />
            <Vertex X1="70.6733" X2="-29.9346" />
            <Vertex X1="70.6519" X2="-30.1517" />
            <Vertex X1="70.6258" X2="-30.3683" />
            <Vertex X1="70.595" X2="-30.5843" />
            <Vertex X1="70.5594" X2="-30.7996" />
            <Vertex X1="70.5192" X2="-31.014" />
            <Vertex X1="70.4743" X2="-31.2275" />
            <Vertex X1="70.4248" X2="-31.4399" />
            <Vertex X1="70.3706" X2="-31.6513" />
            <Vertex X1="70.3119" X2="-31.8614" />
            <Vertex X1="70.2485" X2="-32.0701" />
            <Vertex X1="70.1807" X2="-32.2775" />
            <Vertex X1="70.1083" X2="-32.4833" />
            <Vertex X1="70.0314" X2="-32.6875" />
            <Vertex X1="69.9502" X2="-32.8899" />
            <Vertex X1="69.8645" X2="-33.0905" />
            <Vertex X1="69.7744" X2="-33.2893" />
            <Vertex X1="69.6801" X2="-33.486" />
            <Vertex X1="69.5815" X2="-33.6806" />
            <Vertex X1="69.4786" X2="-33.873" />
            <Vertex X1="69.3716" X2="-34.0631" />
            <!--Vertex X1="69.3716" X2="-34.0631" /-->
          </LinPoly>
        </Primitives>
      </ConductingSheet>
      <ConductingSheet Name="MS36" Conductivity="45454545.4545455" Thickness="3.5e-05">
        <Primitives>
          <Box Priority="2">
            <P1 X="40.5774" Y="-42.5631" Z="0" />
            <P2 X="50.5774" Y="-41.5631" Z="0" />
          </Box>
          <Box Priority="2">
            <P1 X="40.5774" Y="-40.5631" Z="0" />
            <P2 X="50.5774" Y="-39.5631" Z="0" />
          </Box>
        </Primitives>
      </ConductingSheet>
      <ConductingSheet Name="MS40" Conductivity="45454545.4545455" Thickness="3.5e-05">
        <Primitives>
          <Box Priority="2">
            <P1 X="51.5774" Y="-44.5631" Z="0" />
            <P2 X="61.5774" Y="-43.5631" Z="0" />
          </Box>
          <Box Priority="2">
            <P1 X="51.5774" Y="-42.5631" Z="0" />
            <P2 X="61.5774" Y="-41.5631" Z="0" />
          </Box>
        </Primitives>
      </ConductingSheet>
      <ConductingSheet Name="MS39" Conductivity="45454545.4545455" Thickness="3.5e-05">
        <Primitives>
          <Box Priority="2">
            <P1 X="29.5774" Y="-44.5631" Z="0" />
            <P2 X="39.5774" Y="-43.5631" Z="0" />
          </Box>
          <Box Priority="2">
            <P1 X="29.5774" Y="-42.5631" Z="0" />
            <P2 X="39.5774" Y="-41.5631" Z="0" />
          </Box>
        </Primitives>
      </ConductingSheet>
      <ConductingSheet Name="MS1" Conductivity="45454545.4545455" Thickness="3.5e-05">
        <Primitives>
          <Box Priority="2">
            <P1 X="20.1" Y="-78.0631" Z="0" />
            <P2 X="30.1" Y="-77.0631" Z="0" />
          </Box>
        </Primitives>
      </ConductingSheet>
      <ConductingSheet Name="MS3" Conductivity="45454545.4545455" Thickness="3.5e-05">
        <Primitives>
          <Box Priority="2">
            <P1 X="30.1" Y="-78.5631" Z="0" />
            <P2 X="40.1" Y="-76.5631" Z="0" />
          </Box>
        </Primitives>
      </ConductingSheet>
      <ConductingSheet Name="MS6" Conductivity="45454545.4545455" Thickness="3.5e-05">
        <Primitives>
          <Box Priority="2">
            <P1 X="56.1" Y="-76.5631" Z="0" />
            <P2 X="54.1" Y="-66.5631" Z="0" />
          </Box>
        </Primitives>
      </ConductingSheet>
      <ConductingSheet Name="MS7" Conductivity="45454545.4545455" Thickness="3.5e-05">
        <Primitives>
          <Box Priority="2">
            <P1 X="56.1" Y="-78.5631" Z="0" />
            <P2 X="66.1" Y="-76.5631" Z="0" />
          </Box>
        </Primitives>
      </ConductingSheet>
      <ConductingSheet Name="MS8" Conductivity="45454545.4545455" Thickness="3.5e-05">
        <Primitives>
          <Box Priority="2">
            <P1 X="54.1" Y="-66.5631" Z="0" />
            <P2 X="56.1" Y="-64.5631" Z="0" />
          </Box>
        </Primitives>
      </ConductingSheet>
      <ConductingSheet Name="MS9" Conductivity="45454545.4545455" Thickness="3.5e-05">
        <Primitives>
          <Box Priority="2">
            <P1 X="66.1" Y="-78.5631" Z="0" />
            <P2 X="68.1" Y="-76.5631" Z="0" />
          </Box>
        </Primitives>
      </ConductingSheet>
      <ConductingSheet Name="MS12" Conductivity="45454545.4545455" Thickness="3.5e-05">
        <Primitives>
          <Box Priority="2">
            <P1 X="68.1" Y="-76.5631" Z="0" />
            <P2 X="66.1" Y="-66.5631" Z="0" />
          </Box>
        </Primitives>
      </ConductingSheet>
      <ConductingSheet Name="MS13" Conductivity="45454545.4545455" Thickness="3.5e-05">
        <Primitives>
          <LinPoly Priority="2" Elevation="0" Length="0" NormDir="2">
            <Vertex X1="40.1" X2="-78.5631" />
            <Vertex X1="44.1" X2="-78.5631" />
            <Vertex X1="44.1" X2="-77.0631" />
            <Vertex X1="42.1" X2="-77.0631" />
            <Vertex X1="42.1" X2="-76.5631" />
            <Vertex X1="40.1" X2="-76.5631" />
          </LinPoly>
        </Primitives>
      </ConductingSheet>
      <ConductingSheet Name="MS14" Conductivity="45454545.4545455" Thickness="3.5e-05">
        <Primitives>
          <LinPoly Priority="2" Elevation="0" Length="0" NormDir="2">
            <Vertex X1="54.1" X2="-76.5631" />
            <Vertex X1="56.1" X2="-76.5631" />
            <Vertex X1="56.1" X2="-78.5631" />
            <Vertex X1="54.6" X2="-78.5631" />
            <Vertex X1="54.6" X2="-78.0631" />
            <Vertex X1="54.1" X2="-78.0631" />
          </LinPoly>
        </Primitives>
      </ConductingSheet>
      <ConductingSheet Name="MS18" Conductivity="45454545.4545455" Thickness="3.5e-05">
        <Primitives>
          <LinPoly Priority="2" Elevation="0" Length="0" NormDir="2">
            <Vertex X1="42.8071" X2="-89.5631" />
            <Vertex X1="41.3929" X2="-89.5631" />
            <Vertex X1="35.0289" X2="-95.927" />
            <!--Vertex X1="35.0289" X2="-95.927" /-->
            <Vertex X1="35.2641" X2="-96.1546" />
            <Vertex X1="35.5065" X2="-96.3744" />
            <Vertex X1="35.7561" X2="-96.5861" />
            <Vertex X1="36.0124" X2="-96.7895" />
            <Vertex X1="36.2752" X2="-96.9844" />
            <Vertex X1="36.5443" X2="-97.1707" />
            <Vertex X1="36.8193" X2="-97.348" />
            <Vertex X1="37.1" X2="-97.5162" />
            <Vertex X1="37.386" X2="-97.6752" />
            <Vertex X1="37.6771" X2="-97.8247" />
            <Vertex X1="37.9729" X2="-97.9646" />
            <Vertex X1="38.2732" X2="-98.0948" />
            <Vertex X1="38.5775" X2="-98.215" />
            <Vertex X1="38.8856" X2="-98.3253" />
            <Vertex X1="39.1972" X2="-98.4254" />
            <Vertex X1="39.5118" X2="-98.5152" />
            <Vertex X1="39.8292" X2="-98.5947" />
            <Vertex X1="40.1491" X2="-98.6638" />
            <Vertex X1="40.471" X2="-98.7224" />
            <Vertex X1="40.7947" X2="-98.7704" />
            <Vertex X1="41.1198" X2="-98.8078" />
            <Vertex X1="41.446" X2="-98.8346" />
            <Vertex X1="41.7728" X2="-98.8506" />
            <Vertex X1="42.1" X2="-98.856" />
            <Vertex X1="42.4272" X2="-98.8506" />
            <Vertex X1="42.754" X2="-98.8346" />
            <Vertex X1="43.0802" X2="-98.8078" />
            <Vertex X1="43.4053" X2="-98.7704" />
            <Vertex X1="43.729" X2="-98.7224" />
            <Vertex X1="44.0509" X2="-98.6638" />
            <Vertex X1="44.3708" X2="-98.5947" />
            <Vertex X1="44.6882" X2="-98.5152" />
            <Vertex X1="45.0028" X2="-98.4254" />
            <Vertex X1="45.3144" X2="-98.3253" />
            <Vertex X1="45.6225" X2="-98.215" />
            <Vertex X1="45.9268" X2="-98.0948" />
            <Vertex X1="46.2271" X2="-97.9646" />
            <Vertex X1="46.5229" X2="-97.8247" />
            <Vertex X1="46.814" X2="-97.6752" />
            <Vertex X1="47.1" X2="-97.5162" />
            <Vertex X1="47.3807" X2="-97.348" />
            <Vertex X1="47.6557" X2="-97.1707" />
            <Vertex X1="47.9248" X2="-96.9844" />
            <Vertex X1="48.1876" X2="-96.7895" />
            <Vertex X1="48.4439" X2="-96.5861" />
            <Vertex X1="48.6935" X2="-96.3744" />
            <Vertex X1="48.9359" X2="-96.1546" />
            <Vertex X1="49.1711" X2="-95.927" />
            <!--Vertex X1="49.1711" X2="-95.927" /-->
          </LinPoly>
        </Primitives>
      </ConductingSheet>
      <ConductingSheet Name="MS4" Conductivity="45454545.4545455" Thickness="3.5e-05">
        <Primitives>
          <Box Priority="2">
            <P1 X="44.1" Y="-88.5631" Z="0" />
            <P2 X="40.1" Y="-78.5631" Z="0" />
          </Box>
        </Primitives>
      </ConductingSheet>
      <ConductingSheet Name="MS15" Conductivity="45454545.4545455" Thickness="3.5e-05">
        <Primitives>
          <LinPoly Priority="2" Elevation="0" Length="0" NormDir="2">
            <Vertex X1="44.1" X2="-88.5631" />
            <Vertex X1="40.1" X2="-88.5631" />
            <Vertex X1="40.1" X2="-89.5631" />
            <Vertex X1="44.1" X2="-89.5631" />
          </LinPoly>
        </Primitives>
      </ConductingSheet>
      <ConductingSheet Name="MS19" Conductivity="45454545.4545455" Thickness="3.5e-05">
        <Primitives>
          <Box Priority="2">
            <P1 X="44.1" Y="-89.5631" Z="0" />
            <P2 X="54.1" Y="-88.5631" Z="0" />
          </Box>
        </Primitives>
      </ConductingSheet>
      <ConductingSheet Name="MS20" Conductivity="45454545.4545455" Thickness="3.5e-05">
        <Primitives>
          <Box Priority="2">
            <P1 X="30.1" Y="-89.5631" Z="0" />
            <P2 X="40.1" Y="-88.5631" Z="0" />
          </Box>
        </Primitives>
      </ConductingSheet>
      <ConductingSheet Name="MS5" Conductivity="45454545.4545455" Thickness="3.5e-05">
        <Primitives>
          <Box Priority="2">
            <P1 X="44.1" Y="-78.0631" Z="0" />
            <P2 X="54.1" Y="-77.0631" Z="0" />
          </Box>
        </Primitives>
      </ConductingSheet>
      <ConductingSheet Name="MS22" Conductivity="45454545.4545455" Thickness="3.5e-05">
        <Primitives>
          <LinPoly Priority="2" Elevation="0" Length="0" NormDir="2">
            <Vertex X1="66.1" X2="-66.5631" />
            <Vertex X1="68.1" X2="-66.5631" />
            <Vertex X1="66.1" X2="-64.5631" />
          </LinPoly>
        </Primitives>
      </ConductingSheet>
      <ConductingSheet Name="MS23" Conductivity="45454545.4545455" Thickness="3.5e-05">
        <Primitives>
          <LinPoly Priority="2" Elevation="0" Length="0" NormDir="2">
            <Vertex X1="55.6" X2="-83.5881" />
            <Vertex X1="54.6" X2="-83.5881" />
            <Vertex X1="55.6" X2="-84.5881" />
          </LinPoly>
        </Primitives>
      </ConductingSheet>
      <ConductingSheet Name="MS21" Conductivity="45454545.4545455" Thickness="3.5e-05">
        <Primitives>
          <Box Priority="2">
            <P1 X="55.6" Y="-83.5881" Z="0" />
            <P2 X="54.6" Y="-78.5631" Z="0" />
          </Box>
        </Primitives>
      </ConductingSheet>
      <ConductingSheet Name="MS11" Conductivity="45454545.4545455" Thickness="3.5e-05">
        <Primitives>
          <Box Priority="2">
            <P1 X="56.1" Y="-66.5631" Z="0" />
            <P2 X="66.1" Y="-64.5631" Z="0" />
          </Box>
        </Primitives>
      </ConductingSheet>
      <ConductingSheet Name="MS24" Conductivity="45454545.4545455" Thickness="3.5e-05">
        <Primitives>
          <Box Priority="2">
            <P1 X="55.6" Y="-84.5881" Z="0" />
            <P2 X="65.6" Y="-83.5881" Z="0" />
          </Box>
        </Primitives>
      </ConductingSheet>
      <ConductingSheet Name="MS41" Conductivity="45454545.4545455" Thickness="3.5e-05">
        <Primitives>
          <Box Priority="2">
            <P1 X="66.1" Y="-84.5881" Z="-1" />
            <P2 X="65.1" Y="-83.5881" Z="0" />
          </Box>
        </Primitives>
      </ConductingSheet>
      <LumpedElement Name="port_resist_1" Direction="2" Caps="1" R="50">
        <Primitives>
          <Box Priority="5">
            <P1 X="20" Y="-78.0631" Z="-1" />
            <P2 X="20.2" Y="-77.0631" Z="0" />
          </Box>
        </Primitives>
      </LumpedElement>
      <LumpedElement Name="port_resist_2" Direction="2" Caps="1" R="50">
        <Primitives>
          <Box Priority="5">
            <P1 X="29.4774" Y="-42.5631" Z="-1" />
            <P2 X="29.6774" Y="-41.5631" Z="0" />
          </Box>
        </Primitives>
      </LumpedElement>
      <Excitation Name="port_excite_1" Type="0" Excite="-0,-0,-1">
        <Primitives>
          <Box Priority="5">
            <P1 X="20" Y="-78.0631" Z="-1" />
            <P2 X="20.2" Y="-77.0631" Z="0" />
          </Box>
        </Primitives>
      </Excitation>
      <ProbeBox Name="port_ut1" Type="0" Weight="-1">
        <Primitives>
          <Box Priority="5">
            <P1 X="20.1" Y="-77.5631" Z="-1" />
            <P2 X="20.1" Y="-77.5631" Z="0" />
          </Box>
        </Primitives>
      </ProbeBox>
      <ProbeBox Name="port_it1" Type="1" Weight="1" NormDir="2">
        <Primitives>
          <Box Priority="5">
            <P1 X="20" Y="-78.0631" Z="-0.5" />
            <P2 X="20.2" Y="-77.0631" Z="-0.5" />
          </Box>
        </Primitives>
      </ProbeBox>
      <ProbeBox Name="port_ut2" Type="0" Weight="-1">
        <Primitives>
          <Box Priority="5">
            <P1 X="29.5774" Y="-42.0631" Z="-1" />
            <P2 X="29.5774" Y="-42.0631" Z="0" />
          </Box>
        </Primitives>
      </ProbeBox>
      <ProbeBox Name="port_it2" Type="1" Weight="1" NormDir="2">
        <Primitives>
          <Box Priority="5">
            <P1 X="29.4774" Y="-42.5631" Z="-0.5" />
            <P2 X="29.6774" Y="-41.5631" Z="-0.5" />
          </Box>
        </Primitives>
      </ProbeBox>
    </Properties>
    <RectilinearGrid DeltaUnit="0.001" CoordSystem="0">
      <XLines>10.714196,12.857024,15.000000,17.487449,18.752007,19.394613,19.721164,19.887106,19.971432,20.014284,20.057140,20.100000,20.142860,20.185716,20.228568,20.311647,20.472717,20.784991,21.390415,22.564183,24.838700,26.947199,28.108294,28.746893,29.098120,29.291294,29.397540,29.455974,29.488113,29.520255,29.552400,29.582401,29.612403,29.642397,29.688091,29.757701,29.863700,29.952356,30.040964,30.129518,30.300192,30.629141,31.263140,32.485077,34.838700,37.094174,38.314635,38.974834,39.331963,39.525149,39.629651,39.734176,39.838700,39.936689,40.034680,40.132660,40.233969,40.338700,40.434184,40.529668,40.625132,40.771392,40.995475,41.338700,41.664989,41.991257,42.317487,42.685317,43.100000,43.528595,43.957161,44.385678,45.019054,45.955223,47.338700,48.752353,49.619546,50.151322,50.477417,50.677383,50.877392,51.077400,51.277408,51.477417,51.677383,52.069593,52.838700,53.506891,53.861669,54.050008,54.149992,54.249996,54.350000,54.443796,54.537541,54.631229,54.799137,55.100000,55.400863,55.568771,55.662459,55.756204,55.850000,55.957149,56.064290,56.171420,56.373705,56.755669,57.476908,58.838700,60.356388,61.357383,62.017434,62.677889,63.338700,64.119804,64.627383,64.957161,65.171420,65.385702,65.600000,65.800008,66.000017,66.199983,66.535875,67.100000,67.528595,67.957161,68.385678,69.059568,70.119332,71.785926,74.405700,77.108292,79.810699</XLines>
      <YLines>-107.478109,-103.343972,-99.209550,-94.647291,-92.171892,-90.829225,-90.100956,-89.705939,-89.491680,-89.277398,-89.063100,-88.848802,-88.634520,-88.420261,-88.071139,-87.502265,-86.575600,-85.648935,-85.080061,-84.730939,-84.516680,-84.302398,-84.088100,-83.873802,-83.659520,-83.445261,-83.056787,-82.352441,-81.075600,-79.856671,-79.192748,-78.831298,-78.634520,-78.527390,-78.420249,-78.313100,-78.219304,-78.125559,-78.031871,-77.863963,-77.563100,-77.262237,-77.094329,-77.000641,-76.906896,-76.813100,-76.705951,-76.598810,-76.491680,-76.292387,-75.921638,-75.231930,-73.948860,-71.563100,-69.026525,-67.624019,-66.848778,-66.420261,-65.991695,-65.563100,-65.134505,-64.705939,-64.277422,-63.491565,-62.050386,-59.407412,-54.563100,-49.791581,-47.225440,-45.846024,-45.104527,-44.705939,-44.491680,-44.277398,-44.063100,-43.833142,-43.625559,-43.438183,-43.250692,-43.063100,-42.875508,-42.688017,-42.500641,-42.293058,-42.063100,-41.833142,-41.625559,-41.438183,-41.250692,-41.063100,-40.875508,-40.688017,-40.500641,-40.293058,-40.063100,-39.848802,-39.634520,-39.420261,-39.027333,-38.306743,-36.985256,-34.563100,-32.140944,-30.819457,-30.098867,-29.705939,-29.491680,-29.277398,-29.063100,-28.848802,-28.634520,-28.420261,-28.029019,-27.314600,-26.010051,-23.627910,-19.281550,-15.303523,-11.325769</YLines>
      <ZLines>-0.928580,-0.714298,-0.500000,-0.285702,-0.071420</ZLines>
    </RectilinearGrid>
  </ContinuousStructure>
</openEMS>
