import React, { useState } from 'react';
import axios from 'axios';
import './App.css';

function App() {
  const [comando, setComando] = useState('');
  const [resultado, setResultado] = useState('');
  const [historial, setHistorial] = useState([]);

  const ejecutarComando = async () => {
    if (!comando.trim()) {
      setResultado('Error: Ingrese un comando');
      return;
    }

    try {
      const response=await axios.post('/analyze', comando, {
      headers: {'Content-Type': 'text/plain'}
    });
      const texto =response.data;
      setResultado(texto);
      setHistorial([...historial, { comando, resultado: texto }]);
    }
    catch (error) {
      setResultado('Error al conectar con el backend. Asegúrate de que el servidor esté corriendo en http://localhost:8080');
    }
  };

  return (
    <div className="App">
      <h1>Analizador de Comandos EXT2</h1>
      <div className="entrada">
        <textarea
          rows="4"
          value={comando}
          onChange={(e) => setComando(e.target.value)}
          placeholder="Ingrese un comando (ej: mkdisk -size=10 -path=/tmp/disco.mia)"
        />
        <br />
        <button onClick={ejecutarComando}>Ejecutar</button>
      </div>
      <div className="salida">
        <h3>Resultado:</h3>
        <pre>{resultado}</pre>
      </div>
      <div className="historial">
        <h3>Historial de comandos</h3>
        {historial.map((item, index) => (
          <div key={index} className="historial-item">
            <strong>Comando:</strong> {item.comando}<br />
            <strong>Resultado:</strong> {item.resultado}
            <hr />
          </div>
        ))}
      </div>
    </div>
  );
}
export default App;

